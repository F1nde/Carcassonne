#include "configuration.hh"
#include "constants.hh"

#include <QFile>
#include <QString>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QCoreApplication>
#include <exception>

namespace TIE2400 {


Configuration::Configuration() : initDone_( false )
{

}

// rakentajassa luetaan tiedosto ja jäsennetään
// sen sisältö JSON-muodosta olion sisälle
// talteen avain-arvo -pareiksi (map)
void Configuration::init(QByteArray data)
{
    Q_ASSERT( initDone_ == false ); // alustus vain kerran

    // yritetään lukea parametridata QJsonDocument-olioksi
    // epäonnistuminen lopettaa ohjelman suoritukset
    QJsonParseError errors;
    auto doc = QJsonDocument::fromJson( data, &errors );
    if( errors.error != QJsonParseError::ParseError::NoError ) {
        qDebug() << errors.errorString() << "\nat:" << errors.offset;
        qDebug() << "JSON parse error";
        QCoreApplication::quit();
    }
    //file.close();

    qDebug() << doc;  // tulostetaan luettu JSON

    // vaaditaan, että JSON-tiedosto sisältää yhden JSON-olion
    if( doc.isObject() != true ){
        qDebug() << "JSON does not contain an object";
        QCoreApplication::quit();
    }

    // jokaisen avain-arvo -parin JSON-tiedoissa tulee sisältää
    // merkkijonoja
    auto obj = doc.object();
    for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        QString key = i.key();
        QJsonValue value = i.value();
        if( not value.isString() ) {
            qDebug() << key << " value is not a string";
            qDebug() << "JSON item is not a string";
            QCoreApplication::quit();
        }
        // save accepted key / value pairs:
        data_.insert( key, value.toString() );
    } // foreach key/value

    initDone_ = true;
}


// palautetaan avaimella 'key' talletettu merkkijonoarvo
std::string Configuration::getString(std::string key)
{
    Q_ASSERT( initDone_ == true );

    QString qsKey = QString::fromStdString(key);
    if( data_.contains(qsKey) ) {
        QString item = data_[ qsKey ];
        return item.toStdString();
    } else {
        throw ConfigError( std::string("getString: No such key: ") + key );
    }
}

void Configuration::setString(std::string key, std::string value)
{
    Q_ASSERT( initDone_ == true );
    auto qkey = QString::fromStdString( key );
    auto qval = QString::fromStdString( value );
    data_.insert( qkey, qval );
}

// palautetaan avaimella 'key' talletettu kokonaisluku
int Configuration::getInt(std::string key)
{
    Q_ASSERT( initDone_ == true );
    QString qsKey = QString::fromStdString(key);
    if( data_.contains(qsKey) ) {
        QString item = data_[ qsKey ];

        // sisäinen talletusmuoto on merkkijono, joten
        // muutetaan se kokonaisluvuksi ennen palautusta
        bool ok = false;
        int value = item.toInt(&ok, 10);
        if( not ok )
            throw ConfigError( std::string("getInt: value is not integer: ")
                               + item.toStdString());
        else
            return value;
    } else {
        throw ConfigError( std::string("getInt: No such key:") + key );
    }

}

void Configuration::setInt(std::string key, int value)
{
    Q_ASSERT( initDone_ == true );
    auto qkey = QString::fromStdString( key );
    auto qval = QString("%1").arg( value );
    data_.insert( qkey, qval );
}

} // namespace TIE2400
