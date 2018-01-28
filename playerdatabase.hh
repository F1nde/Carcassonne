#ifndef PLAYERDATABASE_HH
#define PLAYERDATABASE_HH

#include <string>
#include <vector>
#include <QColor>
#include <QMap>
#include <QString>

/*!
 * \brief PlayerDataBase-luokka
 *
 *  Tama luokka huolehtii pelaajien varien ja nappulamaarien pitamisesta
 *  tietorakenteessa. Lisaksi sen julkisen rajapinnan kautta voidaan lisata
 *  tietorakenteeseen pelaajia, vahentaa heidan pisteitaan seka vaihtaa
 *  vuorossa olevaa pelaajaa.
 **/

class playerDataBase
{
public:
    /*!
    * \brief Rakentaja pelaajatietokannan, johon on alustettu paikat annetulle maaralle pelaajia
    * \param players pelaajien maara
    * \pre 1 <= players <= 6
    **/
    playerDataBase( int players );

    /*!
    * \brief Purkaja purkaa pelaajatietokannan
    * \pre -
    **/
    ~playerDataBase();

    /*!
     * \brief Lisaa pelaajan pelaajatietokantaan
     * \param color lisattavan pelaajan nappuloiden vari
     * \pre QColor loytyy playerColors vektorista
     * \post Pelaaja on lisatty pelaajatietokantaan
     */
    void addPlayer( QColor color );

    /*!
     * \brief Vaihdetaan vuorossa oleva pelaaja ja palautetaan seuraavaksi vuorossa olevan pelaajan numero
     * \return Juuri vuoroon tulleen pelaajan numero string:ina
     * \pre -
     * \post Vaihdettu vuorossa oleva pelaaja
     */
    std::string switchPlayer();

    /*!
     * \brief Palauttaa pelaajan nappula maaran
     * \param color on pelaajan nappuloiden vari
     * \return int tokens
     * \pre vari loytyy tokens_ map-tietorakenteesta
     */
    int getTokens( QColor color );

    /*!
     * \brief Tarkistaa onko peli paattynyt
     * \return bool onko peli paattynyt
     * \pre -
     */
    bool checkIfEnds();

    /*!
     * \brief Vahentaa pelaajalta yhden nappulan, jos pelaajalla on nappuloita jaljella
     * \param color on pelaajan nappuloiden vari
     * \pre vari loytyy tokens_ map-tietorakenteesta
     * \post varin omistavalta pelaajalta on vahennetty yksi nappula, jos hanella on nappuloita jaljella
     */
    void changeTokens( QColor color );

private:

    QMap< QString, int > tokens_;       //!< Tietorakenne, jossa pelaajien nappulamaarat
    size_t currentPlayer_;              //!< Vuorossa olevan pelaajan indeksi tietorakenteessa
    size_t maxPlayers_;                 //!< Pelaajien maara

    /*!
     * \class playerDataBase
     * \internal
     * \invariant 0 < currentPlayer_ && currentPlayer_ <= maxPlayers_
     **/

    /*!
     * \brief invariantti testaa luokkainvariantin
     */
    void invariant() const;
};

#endif // PLAYERDATABASE_HH
