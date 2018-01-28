
#include "ui_config.h"
#include "ui_endscreen.h"

#include "configuration.hh"
#include "gameconstants.hh"
#include "mygameengine.hh"
#include "pointcard.hh"
#include "statusscreen.hh"

#include <algorithm>
#include <QDebug>
#include <QDialog>
#include <QObject>
#include <QPainter>
#include <QPalette>
#include <QString>
#include <QTextEdit>

//
// Kun peli tarvitsee moottoria (joka toteuttaa IGameEngine rajapinnan),
// niin se kutsuu tätä funktiota. Meidän toteutuksemme luo oman oliomme
// ja palauttaa unique_ptr-osoittimen siihen.
// Näin muun osan (GameUI) ei tarvitse edes tietää minkä niminen toteutusluokka on -
// riittää, että olio toteuttaa halutun rajapinnan.
//
std::unique_ptr<TIE2400::IGameEngine> TIE2400::createGameEngine()
{
    return std::unique_ptr<TIE2400::IGameEngine>( new myEngine() );
}

// rakentajassa annetaan alkuarvot kaikille jäsenmuuttujille ja
// kutsutaan Qt:n resource-mekanismin alustusta pelikorttien kuville
myEngine::myEngine() : dialog_(nullptr), ui_dialog_(nullptr), ui_statusScreen_(nullptr),
    ui_endscreen_(nullptr), view_(nullptr),
    cardImages_(), deck_(), currentCard_(nullptr), cardRotation_(0),
    gameBoard_(nullptr), dataBase_(nullptr), rd_(), re_( rd_() ), pickedColors_(6),
    pickedQColors_(), pickedNames_()
{
    // osana kirjastoa (static lib) olevat resurssit pitaa itse alustaa
    Q_INIT_RESOURCE(pelikortit);
}

// purkaja
myEngine::~myEngine()
{
    delete ui_dialog_;
    delete ui_endscreen_;
    // dialog_ osoitin on Qt:n hallinnassa, joten sitä ei tarvitse tuhota(?)
}

/*
 * toteutukset IGameEngine-rajapinnan määrittelemille operaatioille
 */

void myEngine::setView( TIE2400::IGameView* v )
{
    Q_ASSERT( v != nullptr );
    view_ = v;
}

void myEngine::startConfiguration(QWidget* parent)
{
    Q_ASSERT( view_ != nullptr );  // setView pitää olla kutsuttuna ennen tätä jäsenfunktiota

    // Tehdään korttikuvista (png) valmiiksi pixmap-kuvia
    // tätä ei tehdä rakentajassa, koska Qt pitää olla alustettuna
    // ennen QPixmap:n rutiinien käyttöä
    for( int i = 1; i <= 5; i++ ) {
        auto cardName = QString(":/cards/card%1").arg(i);
        QImage card( cardName );
        if( card.isNull() ) {
            qDebug() << "cannot read card";
            QCoreApplication::quit();
        }
        auto pix  = QPixmap::fromImage( card );
        cardImages_.push_back( pix );
    }

    // luomme QDialog-olion, jonka sisälle
    // alustetaan QtDesignerilla tehty käyttöliittymä (UI::Config)
    dialog_ = new QDialog( parent );
    ui_dialog_ = new Ui::Config();
    ui_dialog_->setupUi( dialog_ );

    // Luodaan yhteyksia UI:n interaktivoimiseksi
    // Pelaajamaaran muuttamisen connect
    QObject::connect(ui_dialog_->playerAmountSlider, SIGNAL(valueChanged(int)), this, SLOT(on_playerAmountSlider_sliderMoved(int)));

    // Rakennetaan pelaajien varivalikoimat valmiiksi
    std::vector<QComboBox*> colorBoxes {ui_dialog_->player1color,ui_dialog_->player2color,ui_dialog_->player3color,
                ui_dialog_->player4color,ui_dialog_->player5color,ui_dialog_->player6color};
    QComboBox* temp = nullptr;
    for(int playerStepper = 0; playerStepper < static_cast<int>(colorBoxes.size()); ++playerStepper)
    {
        temp = colorBoxes.at(playerStepper);
        for( int colorStepper = 0; colorStepper < static_cast<int>(gameConstants::playerColorNames.size())+1; ++colorStepper)
        {
            if( colorStepper == 0)
            {
                temp->insertItem(colorStepper, "Choose a color...");
            }
            else
            {
                temp->insertItem(colorStepper, gameConstants::playerColorNames.at(colorStepper - 1));
            }
        }
    }
    
    // Yhteys pelaajien varien valinnan ja enginen valille
    QObject::connect(ui_dialog_->player1color, SIGNAL(currentIndexChanged(int)), this, SLOT(player1colorPicked(int)));
    QObject::connect(ui_dialog_->player2color, SIGNAL(currentIndexChanged(int)), this, SLOT(player2colorPicked(int)));
    QObject::connect(ui_dialog_->player3color, SIGNAL(currentIndexChanged(int)), this, SLOT(player3colorPicked(int)));
    QObject::connect(ui_dialog_->player4color, SIGNAL(currentIndexChanged(int)), this, SLOT(player4colorPicked(int)));
    QObject::connect(ui_dialog_->player5color, SIGNAL(currentIndexChanged(int)), this, SLOT(player5colorPicked(int)));
    QObject::connect(ui_dialog_->player6color, SIGNAL(currentIndexChanged(int)), this, SLOT(player6colorPicked(int)));

    // exec-jäsenfunktio käynnistää QDialog-käyttöliittymän ja antaa paluuarvona tiedon
    // siitä painoiko käyttäjä OK vai Cancel -nappia
    if( dialog_->exec() == QDialog::Accepted )
    {
        // luodaan valmiiksi dialogi peln loppumista varten (vielä kun parent on hallussa)
        // toimiva ratkaisu
        endDialog_ = new QDialog( parent );
        endDialog_->setWindowTitle("GAME OVER");
        ui_endscreen_ = new Ui::endScreen();
        ui_endscreen_->setupUi( endDialog_ );


        // Pelaajien nimien lahetys ja varastointi
        QString name = ui_dialog_->player1Name->text();
        pickedNames_.push_back(name);
        std::string utf8_text = name.toUtf8().constData();
        TIE2400::conf.setString( "1", utf8_text );

        name = ui_dialog_->player2Name->text();
        pickedNames_.push_back(name);
        utf8_text = name.toUtf8().constData();
        TIE2400::conf.setString( "2", utf8_text );

        name = ui_dialog_->player3Name->text();
        pickedNames_.push_back(name);
        utf8_text = name.toUtf8().constData();
        TIE2400::conf.setString( "3", utf8_text );

        name = ui_dialog_->player4Name->text();
        pickedNames_.push_back(name);
        utf8_text = name.toUtf8().constData();
        TIE2400::conf.setString( "4", utf8_text );

        name = ui_dialog_->player5Name->text();
        pickedNames_.push_back(name);
        utf8_text = name.toUtf8().constData();
        TIE2400::conf.setString( "5", utf8_text );

        name = ui_dialog_->player6Name->text();
        pickedNames_.push_back(name);
        utf8_text = name.toUtf8().constData();
        TIE2400::conf.setString( "6", utf8_text );

        // Pelaajien varien lahetys
        // Varit lahetetaan indekseina gameConstants.hh:n playerColor -vektoriin, voin laittaa jotain muutakin tarvittaessa
        TIE2400::conf.setInt( "1color", ui_dialog_->player1color->currentIndex()-1);
        TIE2400::conf.setInt( "2color", ui_dialog_->player2color->currentIndex()-1);
        TIE2400::conf.setInt( "3color", ui_dialog_->player3color->currentIndex()-1);
        TIE2400::conf.setInt( "4color", ui_dialog_->player4color->currentIndex()-1);
        TIE2400::conf.setInt( "5color", ui_dialog_->player5color->currentIndex()-1);
        TIE2400::conf.setInt( "6color", ui_dialog_->player6color->currentIndex()-1);

        for( int i = 0; i < ui_dialog_->playerAmountSlider->value(); ++i ) {
            pickedQColors_.push_back( gameConstants::playerColors.at( pickedColors_.at(i) - 1 ) );
        }

        // Pelilaudan koko
        TIE2400::conf.setInt( "board_rows", ui_dialog_->rows->value() );
        TIE2400::conf.setInt( "board_columns", ui_dialog_->columns->value() );

        // vaihdetaan käyttöliittymä pelitilaan
        view_->setGameMode();

        // Luodaan pelaajatietokanta ja asetetaan osoitin siihen
        dataBase_.reset( new playerDataBase( ui_dialog_->playerAmountSlider->value()) );

        // Luodaan pelaajat
        for( int i = 0; i < ui_dialog_->playerAmountSlider->value(); ++i ) {
            QColor color = pickedQColors_.at(i); //Laita tahan ui:lta tuleva vari
            dataBase_->addPlayer( color );
        }

        // Luodaan pelilauta ja asetetaan osoitin siihen
        gameBoard_.reset( new gameBoard( ui_dialog_->columns->value(), ui_dialog_->rows->value(), dataBase_ ) );

        // viesti käyttäjälle
        view_->setStatus( std::string("Peli alkaa. Vuorossa ") +
                          TIE2400::conf.getString( dataBase_->switchPlayer() ) + ". Aseta kortti tuplaklikkauksella." );

        // Alustetaan pakka pelaajamaaran ja lisaosavalinnan mukaan
        initializeDeck( ui_dialog_->playerAmountSlider->value(), ui_dialog_->expansionButton->isChecked() );
        qDebug() << "Pakan koko alustuksen jalkeen: " << deck_.size();

        // Luodaan statusScreen ja näytetään se mikäli se halutaan näkyville
        ui_statusScreen_ = new statusScreen(pickedNames_, ui_dialog_->playerAmountSlider->value(), pickedQColors_, deck_.size());
        if( ui_dialog_->statusScreenButton->isChecked() )
        {
            ui_statusScreen_->show();
        }

        currentCard_ = deck_.front();
        deck_.pop_front();

        // Naytetaan ensimmainen kortti pelaajalle
        view_->showCard( currentCard_->getPicture(), getDirection(cardRotation_));

    } else {
        // conf-ikkunassa valittiin "cancel"
        qDebug() << "User abort";
        QCoreApplication::quit();
    }
}

void myEngine::turnLeft()
{
    view_->setStatus( "vasemmalle", 2 );
    qDebug() << "Korttia kaannettiin vasemmalle";

    cardRotation_ += 3;
    view_->showCard( currentCard_->getPicture(), getDirection(cardRotation_) );
}

void myEngine::turnRight()
{
    view_->setStatus( "oikealle", 2 );
    qDebug() << "Korttia kaannettiin oikealle";

    cardRotation_ += 1;
    view_->showCard( currentCard_->getPicture(), getDirection(cardRotation_) );
}

void myEngine::locationClicked( int x, int y )
{
    if( gameBoard_->placeCard( currentCard_, x, y ) ) {
        // Kaannetaan osoittimen paassa olevaa korttia sen verran kun sita on UI:ssa pyoritetty
        // ja asetetaan pelimoottorin laskuri nollaksi
        currentCard_->turn( cardRotation_ % 4 );
        cardRotation_ = 0;

        // Asetetaan kortti nakyville pelilaudalle oikein pain ja päivitetään korttien määrä
        view_->placeCard(x, y, currentCard_->getPicture(), getDirection( currentCard_->getRotation() ));
        ui_statusScreen_->updateCardAmount();

        // Tarkistetaan, syntyiko teitä ja jos syntyi, paivitetaan pelilauta ja statusScreen
        if( gameBoard_->checkRoads( x,y,5 ) == true ){
            updateUIboard();
            for( int stepper = 0; stepper < static_cast<int>(pickedQColors_.size()); ++stepper)
            {
                ui_statusScreen_->setPoints(stepper+1,dataBase_->getTokens(pickedQColors_.at(stepper)));
            }
        };

        // Tarkistetaan pelin loppuminen ja naytetaan tarvittaessa lopetusruutu
        if( dataBase_->checkIfEnds() == true or deck_.size() == 0 ){
            showEndScreen();
        };

        // Otetaan uusi kortti ja poistetaan osoitin pakasta
        currentCard_ = deck_.front();
        deck_.pop_front();

        // Naytetaan uusi kortti ruudulla ja tulostetaan seuraavan pelaajan tuloste
        view_->showCard( currentCard_->getPicture(), getDirection(cardRotation_) );
        view_->setStatus( std::string("Vuorossa pelaaja ") +
                          TIE2400::conf.getString( dataBase_->switchPlayer() ) );
    }

    return;
}


void myEngine::initializeDeck( int p, bool exp )
{ 
    QPixmap pic1;
    QPixmap pic2;
    QPen pen;
    pen.setWidth( gameConstants::tokenAreaWidth );
    QPoint center( TIE2400::CARD_HALF_SIZE, TIE2400::CARD_HALF_SIZE );

    std::shared_ptr<Card> temp;

    if( exp ) {
        // Lisaosa kortit, joita tulee jokaista 1 kappale per pelaaja
        for (int i = 0; i < (gameConstants::CARD_EXP * p); ++i) {
            temp.reset(new Card(cardImages_.at(3), gameConstants::L_EXP_CROSS));
            deck_.push_back(temp);
            temp.reset(new Card(cardImages_.at(4), gameConstants::L_EXP_TCROSS));
            deck_.push_back(temp);
        }
    }

    // Kortit, joissa lapimeno ilman nappulapaikkaa
    for( int i = 0; i < gameConstants::CARD_THROUGH; ++i ) {
        temp.reset(new Card(cardImages_.at(0), gameConstants::L_THROUGH));
        deck_.push_back(temp);
    }

    // Kortit, joissa kaannos ilman nappulapaikkaa
    for( int i = 0; i < gameConstants::CARD_CURVE; ++i ) {
        temp.reset(new Card(cardImages_.at(1), gameConstants::L_CURVE));
        deck_.push_back(temp);
    }

    // Kortit, joissa pelkkia umpikujia
    for( int i = 0; i < gameConstants::CARD_DE; ++i ) {
        temp.reset(new Card(cardImages_.at(2), gameConstants::L_DE));
        deck_.push_back(temp);
    }

    // Pistekortit, joiden maara ja vari maaraytyy pelaajien maaran mukaan
    for( int i = 0; i < p; ++i ) {
        pic1 = cardImages_.at(0);
        QPainter painter1( &pic1 );
        painter1.setBrush( Qt::NoBrush );
        painter1.setRenderHint( QPainter::Antialiasing );
        pen.setColor( pickedQColors_.at(i) );
        painter1.setPen( pen );
        painter1.drawEllipse( center, 5,5 );

        // Maalataan ja lisataan pistekortit, joissa lapimeno
        for( int j = 0; j < gameConstants::CARD_THROUGH_T; ++j ) {
            temp.reset(new pointCard(pic1, gameConstants::L_THROUGH, pickedQColors_.at(i)));
            deck_.push_back(temp);
        }

        pic2 = cardImages_.at(1);
        QPainter painter2( &pic2 );
        painter2.setBrush( Qt::NoBrush );
        painter2.setRenderHint( QPainter::Antialiasing );
        //pen = QPen( gameConstants::playerColors.at(i) );
        painter2.setPen( pen );
        painter2.drawEllipse( center, 5,5 );

        // Maalataan ja lisataan pistekortit, joissa kaannos
        for( int k = 0; k < gameConstants::CARD_CURVE_T; ++k ) {
            temp.reset(new pointCard(pic2, gameConstants::L_CURVE, pickedQColors_.at(i)));
            deck_.push_back(temp);
        }
    }

    // Sekoitetaan pakka
    std::random_shuffle ( deck_.begin(), deck_.end() );

}

TIE2400::CardDirections myEngine::getDirection( unsigned int r )
{
    // Kaannos 0-3 on modulo4 kaannosmaarasta
    unsigned int rotation = r % 4;
    TIE2400::CardDirections direction;

    switch( rotation ) {
    case 0:
        direction = TIE2400::CardDirections::N;
        break;
    case 1:
        direction = TIE2400::CardDirections::E;
        break;
    case 2:
        direction = TIE2400::CardDirections::S;
        break;
    case 3:
        direction = TIE2400::CardDirections::W;
        break;
    }

    return direction;
}

void myEngine::updateUIboard()
{
    // Haetaan pelilaudan leveys ja korkeus
    int width = gameBoard_->getWidth();
    int height = gameBoard_->getHeight();

    for( int y = 1; y <= height; ++y ) {
        for( int x = 1; x <= width; ++x ) {
            // Pyydetaan laudalta osoitin koordinaateissa olevaan korttiin
            std::shared_ptr<Card> temp = gameBoard_->getCardPointer(x, y);
            if( temp != nullptr )
                view_->placeCard(x, y, temp->getPicture(), getDirection( temp->getRotation() ));
        }
    }
}

void myEngine::showEndScreen()
{
    qDebug() << "endScreen käynnistetään-" << ui_dialog_->playerAmountSlider->value();

    // Haetaan lopulliset pisteet vektoriin
    std::vector<int> points;
    for( int stepper = 0; stepper < ui_dialog_->playerAmountSlider->value(); ++stepper )
    {
        points.push_back(ui_statusScreen_->getPoints(stepper+1));
    }

    qDebug() << "Lasketaan pistemäärät";
    // Haetaan alin pistemäärä (voittaja) tai tasapelissä muutetaan endScreenin otsikko
    int lowest = points.at(0);
    int amount = 1;
    for( int stepper = 1; stepper < static_cast<int>(points.size()); ++stepper )
    {
        if( points.at(stepper) < lowest )
        {
            lowest = points.at(stepper);
            amount = 1;
        }
        else if( points.at(stepper) == lowest)
        {
            ++amount;
        }
    }
    if( amount > 1 )
    {
        ui_endscreen_->winnerLabel->setText("TIE! Best players:");
    }

    // Rakennetaan pointteriviidakko endScreeniin
    std::vector<QLabel*> colors {ui_endscreen_->color1Label,ui_endscreen_->color2Label,ui_endscreen_->color3Label,ui_endscreen_->color4Label,ui_endscreen_->color5Label,ui_endscreen_->color6Label};
    std::vector<QLabel*> names {ui_endscreen_->name1Label,ui_endscreen_->name2Label,ui_endscreen_->name3Label,ui_endscreen_->name4Label,ui_endscreen_->name5Label,ui_endscreen_->name6Label};
    QLabel* tempColor = nullptr;
    QLabel* tempName = nullptr;

    qDebug() << "Ratkaistaan voittaja";
    // Tulostetaan voittajat endScreeniin
    int targetLabel = 0;
    for( int stepper = 0; stepper < static_cast<int>(points.size()); ++stepper )
    {
        if( points.at(stepper) == lowest )
        {
            // Nimien kirjoitus
            tempName = names.at(targetLabel);
            tempName->setText(pickedNames_.at(stepper));

            // Värin lisays variruutuun
            tempColor = colors.at(targetLabel);
            QPalette temp;
            temp.setColor(QPalette::Window, pickedQColors_.at(stepper));
            tempColor->setAutoFillBackground(true);
            tempColor->setPalette(temp);

            ++targetLabel;
        }
    }

    // Avataan aiemmin tehty endScreen-ikkuna
    endDialog_->exec();

    // Kun OK -painiketta painetaan, lopetetaan
    qDebug() << "Peli paattyi!";
    QCoreApplication::quit();
}

void myEngine::configOkStatus()
{
    // Funktio joka tarkastaa etta kaikki vaaditut tiedot on annettu pelin kaynnistamiseksi

    // Virhetilat
    bool colorError = false;


    // Testataan ettei loydy samoja vareja tai valitsematonta varia
    for( int stepper1 = 0; stepper1 < static_cast<int>(ui_dialog_->playerAmountSlider->value()) && !colorError; ++stepper1)
    {
        for( int stepper2 = 0; stepper2 < static_cast<int>(ui_dialog_->playerAmountSlider->value()) && !colorError; ++stepper2)
        {
            if(pickedColors_.at(stepper1) <= 0)
            {
                qDebug() << "Värivirhe: joku ei ole valinnut väriä";
                colorError = true;
            }
            else if( stepper1 != stepper2 && pickedColors_.at(stepper1) == pickedColors_.at(stepper2))
            {
                qDebug() << "Värivirhe: kaksi samaa varia: Pelaaja " << stepper1+1 << " ja pelaaja " << stepper2+1;
                colorError = true;
            }
        }
    }

    // Jos virhe -> nappi pois. Muuten nappi nakyvissa
    if( colorError )
    {
        ui_dialog_->okButton->setDisabled(true);
        return;
    }
    ui_dialog_->okButton->setEnabled(true);
}

void myEngine::on_playerAmountSlider_sliderMoved(int position)
{
    qDebug() << "Halutaan muuttaa pelaajamäärä -> " << position;

    // Parempi toteutus
    // Pointterit eri pelaajien nimi- ja varilaatikoihin ovat vektorissa, jotta niita voidaan kayttaa natisti silmukassa
    std::vector<QLineEdit*> playerNameLines {ui_dialog_->player3Name,ui_dialog_->player4Name,ui_dialog_->player5Name,ui_dialog_->player6Name};
    std::vector<QComboBox*> playerColorBoxes {ui_dialog_->player3color,ui_dialog_->player4color,ui_dialog_->player5color,ui_dialog_->player6color};

    QLineEdit* tempName = nullptr;
    QComboBox* tempColor = nullptr;

    // Kaydaan jokainen pelaaja kerralaan lapi onko slideri pienempi kuin testattava arvo
    // DISCLAIMER: Naa tarvii kayda kaikki lapi yks kerrallaan koska slideri voi muuttua vaikka 2->6
    for( int stepper = 0; stepper < static_cast<int>(playerNameLines.size()); stepper++ )
    {
        tempName = playerNameLines.at(stepper);
        tempColor = playerColorBoxes.at(stepper);
        if((position - 3) < stepper )
        {
            tempName->setDisabled(true);
            tempColor->setDisabled(true);
        }
        else
        {
            tempName->setEnabled(true);
            tempColor->setEnabled(true);
        }
    }

    configOkStatus();
}

void myEngine::player1colorPicked(int index)
{
    pickedColors_.at(0) = index;
    qDebug() << "Pelaaja 1 valitsi värin: " << index;
    configOkStatus();
}
void myEngine::player2colorPicked(int index)
{
    pickedColors_.at(1) = index;
    qDebug() << "Pelaaja 2 valitsi värin: " << index;
    configOkStatus();
}
void myEngine::player3colorPicked(int index)
{
    pickedColors_.at(2) = index;
    qDebug() << "Pelaaja 3 valitsi värin: " << index;
    configOkStatus();
}
void myEngine::player4colorPicked(int index)
{
    pickedColors_.at(3) = index;
    qDebug() << "Pelaaja 4 valitsi värin: " << index;
    configOkStatus();
}
void myEngine::player5colorPicked(int index)
{
    pickedColors_.at(4) = index;
    qDebug() << "Pelaaja 5 valitsi värin: " << index;
    configOkStatus();
}
void myEngine::player6colorPicked(int index)
{
    pickedColors_.at(5) = index;
    qDebug() << "Pelaaja 6 valitsi värin: " << index;
    configOkStatus();
}
