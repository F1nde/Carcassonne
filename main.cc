#include "gameui.hh"
#include <QApplication>
#include <QtGlobal>
#include <QFile>
#include <QDebug>
#include "configuration.hh"
#include "constants.hh"

// configuration object
namespace TIE2400 {
    Configuration conf;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Alustetaan konfiguraatio JSON-tiedostosta
    // ensin yritetään avata DATADIR-hakemistosta config.json:
    auto filename = QString::fromStdString( TIE2400::DATADIR + "/" + "config.json");
    QFile infile( filename );
    if( infile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        TIE2400::conf.init( infile.readAll() );
    } else {
        // DATADIR-tiedosto ei aukea, joten yritetään komentorivillä tullutta nimeä
        if( argc > 1 ) {
            QString filename( argv[1] );
            QFile infile( filename );
            if( infile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                TIE2400::conf.init( infile.readAll() );
            } else {
                qDebug() << "cmdline file read failed";
                QCoreApplication::quit();
            }
        } else {
            // ei annettu komentorivillä tiedostonimeä, joten
            // langoitettu oletusdata:
            const char* const jsondata = "{\"board_columns\":\"10\",\"board_rows\":\"10\","
                    "\"player1\":\"pelaaja yksi\",\"player2\":\"pelaaja kaksi\"}";
            QByteArray data( jsondata );
            TIE2400::conf.init( data );
        }
    }

    GameUI w;

    // asetetaan ikkunalle koko ja näytetään käyttäjälle
    w.resize( 600, 500 );
    w.show();

    // siirrytään suorittamaan Qt:n pääsilmukkaa
    return a.exec();
}
