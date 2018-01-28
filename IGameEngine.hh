#ifndef IGAMEENGINE_HH
#define IGAMEENGINE_HH

#include <memory>
#include <QWidget>
#include "IGameView.hh"
#include "constants.hh"

namespace TIE2400 {

/*!
 * \brief IGameEngine-rajapinta pelimoottorille.
 *
 * Rajapinta, joka määrittelee operaatiot, joiden avulla
 * pelinäyttö keskustelee pelilogiikan kanssa.
 *
 * Koko peli voi olla kahdessa tilassa: alustustilassa ja pelitilassa.
 * Ohjelman alussa ollaan alustustilassa ja pelitilaan siirrytään
 * kutsumalla IGameView-rajapinnan operaatiota setGameMode().
 */
class IGameEngine
{
public:      
    virtual ~IGameEngine() = default;

    /*!
     * \brief rajapinnan versionumero
     */
    static const unsigned int VERSION = 1.0;

    /*!
     * \brief alustetaan pelimoottori
     *
     * \pre -
     * \post Pelimoottori on alustustilassa
     */
    IGameEngine() = default;

    /*!
     * \brief kerrotaan osoitin näkymään.
     *
     * pääohjelma (näkymä) kutsuu tätä rutiinia alustusten yhteydessä kertomaan
     * mistä löytyy käyttöliittymärutiinit (määritelty rajapinnassa IGameView)
     *
     * \pre  osoitin v osoittaa IGameView-rajapinnan toteuttavaan olioon, ollaan alustustilassa
     * \post GameEngine voi käyttää annettua näkymää
     */
    virtual void setView( TIE2400::IGameView* v ) = 0;

    /*!
     * \brief pelin alkuarvojen asetus.
     *
     * startConfiguration kutsulla GameEngine saa määritellä toimintaa
     * ohjaavia arvoja ennen pelin käynnistymistä.
     * Graafista konfiguraatioikkunaa varten annetaan osoitin pääikkunaan.
     *
     * \pre näkymä on asetettu (setView), ollaan alustustilassa ja konfiguraatio-olio on olemassa (TIE2400::conf)
     * \post konfiguraatio (TIE2400::conf) on valmis eikä enää muutu. Ollaan siirrytty pelitilaan.
     */
    virtual void startConfiguration( QWidget* ) = 0;


    /*!
     * \brief turnLeft-nappia painettu
     *
     * Käyttöliittymä kertoo, että käyttäjä on painanut "käännä vastapäivään/vasemmalle" -nappia.
     * \pre Näkymä (view) on asetettu setView()-kutsulla, ollaan pelitilassa
     */
    virtual void turnLeft() = 0;

    /*!
     * \brief turnRight-nappia painettu
     *
     * Käyttöliittymä kertoo, että käyttäjä on painanut "käännä myötäpäivään/oikealle" -nappia
     * \pre Näkymä (view) on asetettu setView()-kutsulla ja ollaan pelitilassa
     */
    virtual void turnRight() = 0;

    /*!
     * \brief tiedotus peliruudukon klikkauksesta
     *
     * Kun käyttöliittymässä on tuplaklikattu ruutua
     * niin tähän operaatioon tulee kutsu.
     *
     * \param x ruudun x-koordinaatti
     * \param y ruudun kortin y-koordinaatti
     * \pre aktiivinen kortti on asetettu näkymälle (showCard) ja ollaan pelitilassa
     */
    virtual void locationClicked( int x, int y ) = 0;

};

/*!
 * \brief <a href="">Tehdasfunktio</a> jolla luodaan IGameEngine-rajapinnan toteuttava olio
 */
std::unique_ptr<IGameEngine> createGameEngine();

} // namespace

#endif // IGAMEENGINE_HH
