#include <QIcon>
#include <QGraphicsItem>
#include <QPixmap>
#include <QTimer>
#include <QDebug>

#include "gameui.hh"
#include "ui_mainwindow.h"

#include "constants.hh"
#include "cardslot.hh"
#include "configuration.hh"

#include "ignoreevents.hh"
// apuolio suodattaa kaikki hiiriklikkaukset pois Qt-komponentista, jonka
// emme halua olevan interaktiivinen
static IgnoreEvents ignoreClicks;

// kutsutaan Tehdasfunktiota, joka luo meille pelimoottoriolion,
// joka toteuttaa rajapinnan IGameEngine
#include "IGameEngine.hh"
static std::unique_ptr<TIE2400::IGameEngine> engine = TIE2400::createGameEngine();



// luodaan oliot, jotka muodostavat ruudulle peliruudukon
void GameUI::populateSlots( QGraphicsView* v )
{
    using namespace TIE2400;
    auto *scene = new QGraphicsScene();

    int columns = conf.getInt("board_columns");
    if( columns < 1 or columns > 100 ){
        qDebug() << "board_columns bad value";
        QCoreApplication::quit();
    }

    int rows = conf.getInt("board_rows");
    if( rows < 1 or rows > 100 ){
        qDebug() << "board_rows bad value";
        QCoreApplication::quit();
    }

    int x = 0;
    for(int i = 0; i < columns * CARD_TOTAL_LEN; i += CARD_TOTAL_LEN )
    {
        ++x;
        int y = 0;
        for( int j = 0; j < rows * CARD_TOTAL_LEN; j += CARD_TOTAL_LEN )
        {
            ++y;

            using std::placeholders::_1;
            using std::placeholders::_2;
            checkF_t f = std::bind( &GameUI::doubleClickCard, this, _1, _2);
            auto *item = new CardSlot( x, y, f );
            slots_[ std::tuple<int,int>(x,y) ] = item;  //talletetaan kortin koordinaatit
            item->setPos( QPointF(i, j) );
            scene->addItem( item );
        }
    }

    v->setScene( scene );
}



GameUI::GameUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), view_(nullptr)
{
    // alustetaan UI-työkalulla (QtDesigner) tehty käyttöliittymä
    ui->setupUi(this);

    // muokataan checkbox-komponentteja siten, että ne eivät
    // reagoi hiiren painalluksiin   
    ui->infoInConf->installEventFilter( &ignoreClicks );
    ui->infoInGame->installEventFilter( &ignoreClicks );


    // seuraavan kortin kuvan piirtoalue
    ui->cardView->setScene( new QGraphicsScene() );

    // asetetaan painonapit kutsumaan tämän olion jäsenfunktioita (Qt SLOTs)
    QObject::connect( ui->turnLeft, SIGNAL(clicked()), this, SLOT(rotateLeft()) );
    QObject::connect( ui->turnRight, SIGNAL(clicked()), this, SLOT(rotateRight()) );

    // koska emme halua pelitoimintojen käynnistyvän kun olemme vielä tämän rakentajan
    // koodissa, niin luomme lyhyen ajastimen käynnistämään toiminnot hieman
    // myöhemmin (200 millisekuntia)
    QTimer::singleShot(200, this, SLOT(start()) );

}

GameUI::~GameUI()
{
    delete ui;
}

// SLOTS
void GameUI::start()
{
    // kerrotaan Engine-koodille mitä näkymää sen tulee käyttää (IGameView-rajapinta)
    engine->setView( this );

    // aloitetaan toiminta konfiguraatioikkunalla
    engine->startConfiguration( this );

    // asetusten jälkeen luodaan peliruudukko
    populateSlots( ui->mainView );
}

// käyttäjä haluaa kiertää korttikuvaa
void GameUI::rotateLeft()
{
    engine->turnLeft();
}

// käyttäjä haluaa kiertää korttikuvaa
void GameUI::rotateRight()
{
    engine->turnRight();
}

// apurutiini, jolla käännetään kuva haluttuun suuntaan
QPixmap GameUI::rotate(const QPixmap& card, TIE2400::CardDirections dir)
{
    // valmistellaan kuva, joka on halutussa asennossa
    QTransform t;
    QTransform direction;
    switch( dir ) {
    case TIE2400::CardDirections::N:
        direction = t.rotate(0);
        break;
    case TIE2400::CardDirections::E:
        direction = t.rotate(90);
        break;
    case TIE2400::CardDirections::S:
        direction = t.rotate(180);
        break;
    case TIE2400::CardDirections::W:
        direction = t.rotate(-90);
        break;
    default:
        qDebug() << "card direction error";
        QCoreApplication::quit();
    }

    return card.transformed( direction );
}

//
// IGameView operaatiot
//

void GameUI::setStatus(std::string message, unsigned int time)
{
    // tehdään C++-merkkijonosta QString ja näytetään se
    // käyttöliittymän status-osiossa
    QString msg(message.c_str());
    ui->status->showMessage( msg, time * 1000 );

}

void GameUI::setGameMode( void )
{
    // alustus checkbox pois päältä:
    ui->infoInConf->setChecked( false );
    ui->infoInConf->setEnabled( false );
    // peli checkbox päälle:
    ui->infoInGame->setChecked( true );
    ui->infoInGame->setEnabled( true );
}

void GameUI::showCard(const QPixmap card, TIE2400::CardDirections dir )
{
    // puhdistetaan kortin näyttävä alue
    ui->cardView->scene()->clear();

    // asetetaan näkyville seuraavan kortin grafiikka
    ui->cardView->scene()->addPixmap( rotate( card, dir ) );
}

void GameUI::placeCard(int x, int y, QPixmap pix, TIE2400::CardDirections dir)
{
    auto key = std::tuple<int,int>(x,y);
    Q_ASSERT( slots_.find(key) != slots_.end() ); // koordinaatteja vastaava ruutu pitää olla olemassa
    auto card = slots_[ key ];

    card->usePixmap( rotate( pix, dir ) );
}


// apurutiini, jota ruutuoliot (cardslot) kutsuvat
// tuplaklikkauksen jälkeen
//
void GameUI::doubleClickCard(int x, int y)
{
    engine->locationClicked( x, y );
}
