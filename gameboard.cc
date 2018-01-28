#include "gameboard.hh"
#include "gameconstants.hh"
#include "card.hh"
#include "pointcard.hh"
#include "playerdatabase.hh"

#include <QDebug>
#include <memory>

gameBoard::gameBoard( int width, int height, std::shared_ptr<playerDataBase> dataBase )
{
    firstTurn_ = true;
    dataBase_ = dataBase;
    tile_ tile;
    tile.pointer_ = nullptr;
    tile.lock_ = false;
    std::vector< tile_ > vectorRow ( width, tile );
    std::vector< std::vector< tile_ > > fullVector ( height, vectorRow );
    gameBoard_ = fullVector;
    qDebug() << "Luotiin pelilauta, jonka leveys on " << width << " ja korkeus " << height;

}

gameBoard::~gameBoard()
{
}

bool gameBoard::placeCard( std::shared_ptr<Card> getCard, int x, int y )
{

    // Tarkastetaan, onko kyseessa ensimmainen siirto. Jos ei ole, tarkastetaan,
    // onko ruudussa jo korttia ja etta onko ruudun ymparilla jossain kortti
    if( firstTurn_ || (gameBoard_.at(y - 1).at(x - 1).pointer_ == nullptr
                       && cardAdjacent(x - 1, y - 1) ) ) {

        // heitetaan kortti pointterista kentalle
        gameBoard_.at(y - 1).at(x - 1).pointer_ = getCard;
        if (firstTurn_) { firstTurn_ = false; }

        qDebug() << "Kortti asetettu koordinaatteihin: (" << x << ", " << y << ")";
        return true;
    }

    // Jos kortin asettaminen ei onnistunut, palautetaan false
    return false;
}

bool gameBoard::checkRoads( int x, int y, int direction )
{
    int trues = 0;
    int trueCounter = 0;
    std::vector<bool> links;

    // Jos direction on 5 (= uusi kortti ) hypataan tarkistusten yli
    if( direction != 5 ) {
        if( ( y-1 >= 0 and y <= static_cast<int>( gameBoard_.size() ) ) and (x-1 >= 0 and x <= static_cast<int>(gameBoard_.at(0).size()) ) ) {
            if( gameBoard_.at(y-1).at(x-1).pointer_ == NULL ) {
                return false; // Tarkisteltavassa kohdassa ei ole korttia
            }
        } else {
            return false; // Tarkisteltava ruutu on pelilaudan ulkopuolella
        }
    }

    // Tarkistetaan onko ruudussa lukko
    if( gameBoard_.at(y - 1).at(x - 1).lock_ == true ) {
        return true;
    }

    // Kysytaan kortilta sen linkkitiedot
    links = gameBoard_.at(y-1).at(x-1).pointer_->link();

    // Tarkistetaan onko ruudussa umpikuja
    if( direction != 5 ) {
        direction = gameConstants::comingDirection.at(direction);  
        if( links.at(direction) == false ) {
            return true;
        }
    }

    // Kun tahan asta paastaan, tiedetaan etta kyseessa on eteneva tie tai aloituspala.

    // Selvitetaan kortin linkkejen maara, jolla voidaan tunnistaa kortin tyyppi
    for( int i = 0; i < 4; ++i ) {
        if( links.at(i) == true ) {
            ++trues;
        }
    }

    // Kyseessa on neljan umpikujan kortti -> rekursio jokaiseen suuntaan
    if( trues == 0 ) {
        for( int i = 0; i < 4; ++i ) {
            if( checkRoads( x + gameConstants::compass.at(i).first, y + gameConstants::compass.at(i).second, i) == true ){
                ++trueCounter;
                placeTokens();
                pointcards_.erase( pointcards_.begin(),pointcards_.end() );
                // tie valmistui
            }
        }
        cleaning();
        invariant();
        if( trueCounter != 0) { // Ainakin yksi tie valmistui
            return true;
        }

    } else { // Linkillinen kortti
        int road = 0; // Apumuuttuja pitamaan kirjaa onko umpikujista lahtenyita teita valmistunut
        gameBoard_.at(y - 1).at(x - 1).lock_ = true; // Laitetaan pelilautaan lukko

        for( int i = 0; i < 4; ++i ) { // Loop rekursioiden aloittamisille ja kokoamiselle. 0 = pohjoinen, 1 = ita, 2 = etela, 3 = lansi.
            if( direction == i ) { // Tulosuunta -> trueCounter
                ++trueCounter;
                continue;
            }

            // Jos tarkisteltavassa suunnassa on linkki tai kyseessa on aloituspala -> rekursio
            if( links.at(i) == true or direction == 5 ) {
                size_t temp = pointcards_.size(); // Apumuuttuja umpikujasta lahtevaa tieta varten

                if( checkRoads( x + gameConstants::compass.at(i).first, y + gameConstants::compass.at(i).second, i) == true ) {

                    if( direction == 5 and links.at(i) == false ) { // Tarkistetaan onko kyseessa umpikujasta lahtenyt tie
                        while( temp != pointcards_.size() ) { // Puretaan pistekorttilistasta umpikuja tiesta saadut pointerit
                            qDebug() << "Lisatty nappula.";
                            dataBase_->changeTokens( pointcards_.back()->placeToken() );
                            pointcards_.erase( pointcards_.end()-1, pointcards_.end() );
                        }
                        ++road;
                    } else { // Kyseessa oli linkillinen tie
                        ++trueCounter;
                    }
                }
            }
        }

        if( trues == trueCounter ) { // Linkilla varustettu tie on valmistunut
            if( direction == 5 ) {
                // tie valmistui
                checkPointcard( x , y );
                placeTokens();
                cleaning();
                invariant();
                return true;
            } else { // Palautetaan tieto valmistuneesta tiesta rekursion seuraavalle tasolle
                checkPointcard( x , y );
                return true;
            }
        }

        if( direction == 5 ) {
            cleaning();
            invariant();
            if( road != 0 ) { // Ainakin yksi umpikujasta lahtenyt tie on valmistunut
                return true;
            }
        }
    }

    return false; //Tie(t) ei valmistunut
}

void gameBoard::cleaning()
{
    for( int i = 0; i < getHeight(); ++i){
        for( int j = 0; j < getWidth(); ++j){
            gameBoard_.at(i).at(j).lock_ = false;
        }
    }
    pointcards_.erase( pointcards_.begin(),pointcards_.end() );
}

void gameBoard::checkPointcard( int x ,int y )
{
    std::shared_ptr<Card> temp = gameBoard_.at(y - 1).at(x - 1).pointer_;
    std::shared_ptr<pointCard> pc (std::dynamic_pointer_cast<pointCard>( temp ));
    if ( pc != 0 ) {
        // Jos tultiin tanne, tiedetaan, etta kyseessa oli pointCard
        pointcards_.push_back( pc );
    }
    return;
}

void gameBoard::placeTokens()
{
    for( size_t i = 0; i < pointcards_.size(); ++i ) {
        qDebug() << "Lisatty nappula.";
        dataBase_->changeTokens( pointcards_.at(i)->placeToken() );
    }
    return;
}

int gameBoard::getWidth()
{
    return gameBoard_.at(0).size();
}

int gameBoard::getHeight()
{
    return gameBoard_.size();
}

std::shared_ptr<Card> gameBoard::getCardPointer( int x, int y )
{
    return gameBoard_.at(y - 1).at(x - 1).pointer_;
}

bool gameBoard::cardAdjacent( int x, int y )
{
    for( int i = -1; i < 2; i+=2 ) {
        if( !(x+i < 0 || x+i >= getWidth()) )
            if ( gameBoard_.at(y).at(x + i).pointer_ != nullptr ) { return true; }
        if( !(y+i < 0 || y+i >= getHeight()) )
            if ( gameBoard_.at(y + i).at(x).pointer_ != nullptr ) { return true; }
    }

    return false;
}

void gameBoard::invariant() const
{
    Q_ASSERT( pointcards_.size() == 0 );
    return;
}
