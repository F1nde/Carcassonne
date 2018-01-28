#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include <string>
#include <QMap>

namespace TIE2400 {

/*!
 * \brief minimaalinen poikkeusluokka virheiden "heittoon"
 */
class ConfigError : public std::exception {
public:
    ConfigError(std::string message) : msg_(message) {};
    ~ConfigError() throw() {};
    const char* what() const throw() { return msg_.c_str(); }
private:
    std::string msg_;
};

/*!
 * \brief Configuration-luokka.
 *
 * Tästä luokasta tehty olio TIE2400::conf sisältää pelin käyttämät
 * asetusarvot, jotka on talletettuna avain-arvo -pareina.
 * Olion alustus tehdään lukemalla tiedostosta JSON-muotoinen dokumentti.
 */
class Configuration
{
public:
    // alustusrutiini (sisäiseen käyttöön)
    // bytearray sisältää JSON-dataa
    void init(QByteArray data);

    explicit Configuration( );
    ~Configuration() = default;
    Configuration( const Configuration& ) = delete;
    Configuration operator=( const Configuration& ) = delete;

    /*!
     * \brief hae merkkijono
     * \param key haettavan tiedon nimi
     * \return avainta vastaava tieto tai TIE2400::ConfigError poikkeus jos ei löydy
     */
    std::string getString( std::string key );

    /*!
     * \brief talleta merkkijono
     *
     * Asetetaan talteen merkkijonomuotoinen tieto.
     * \param key  avainnimi tiedolle
     * \param value talletettava tieto merkkijonona
     */
    void setString( std::string key, std::string value );

    /*!
     * \brief hae kokonaisluku
     * \param key haettavan tiedon nimi
     * \return avainta vastaava lukuarvo tai TIE2400::ConfigError poikkeus jos ei löydy tai
     * haettava tieto ei ole luku
     */
    int getInt( std::string key );

    /*!
     * \brief talleta kokonaisluku
     *
     * Asetetaan talteen kokonaisluku.
     * \param key avainnimi tiedolle
     * \param value talletettava tieto kokonaislukuna.
     */
    void setInt( std::string key, int value );

private:
    QMap<QString,QString> data_;
    bool initDone_;
};

/*!
 * ohjelman konfiguraation sisältävä olio
 */
extern Configuration conf;

} // namespace
#endif // CONFIGURATION_HH
