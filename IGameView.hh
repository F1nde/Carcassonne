#ifndef IGAMEVIEW_HH
#define IGAMEVIEW_HH

#include "constants.hh"
#include <string>
#include <QPixmap>

namespace TIE2400 {

/*!
 * \brief IGameView-rajapinta näytölle.
 *
 * Rajapinta, joka määrittelee operaatiot, joiden avulla pelimoottori
 * saa muutettua ruudulla näkyviä asioita.
 *
 * Koko peli voi olla kahdessa tilassa: alustustilassa ja pelitilassa.
 * Ohjelman alussa ollaan alustustilassa ja pelitilaan siirrytään
 * kutsumalla operaatiota setGameMode().
 */
class IGameView
{
public:
    virtual ~IGameView() = default;

    /*!
     * \brief rajapinnan versionumero
     */
    static const unsigned int VERSION = 1.0;

    /*!
     * \brief alustetaan näkymä
     *
     * \pre -
     * \post Näkymä on alustustilassa
     */
    IGameView() = default;

    /*!
     * \brief Näytä merkkijono käyttäjälle.
     *
     * Näytetään parametrina annettu viesti käyttöliittymäikkunan alareunassa (status-alue)
     *
     * \param str Ikkunassa näytettävä viesti
     * \param time viesti pyyhitään pois näin monen sekunnin kuluttua,
     *        nolla == pyyhintää ei tehdä
     */
    virtual void setStatus( std::string str, unsigned int time = 0 ) = 0;

    /*!
     * \brief Vaihda pelitilaan.
     *
     * Vaihdetaan käyttöliittymä alustustilasta pelitilaan
     *
     * \pre Ollaan alustustilassa
     * \post Ollaan pelitilassa
     */
    virtual void setGameMode( void ) = 0;

    /*!
     * \brief seuraavan pelikortin grafiikka.
     *
     * Näytetään pelaajalle  pelikortin kuva.
     * Käyttöliittymä näyttää kortin erillisessä näkymässä ja käyttäjän
     * on tarkoitus valita peliruudukolta paikka mihin tämä kortti sijoitetaan
     *
     * \param p kortin bittikarttakuva
     * \param dir missä suunnassa kortti piirretään.
     *        Suunnassa N kuvaa ei kierretä, muutoin tehdään
     *        kierto ilmansuunnan mukaan.
     * \pre näkymä on alustettu ja ollaan pelitilassa
     * \post korttikuva näytetään käyttäjälle halutussa asennossa
     */
    virtual void showCard( const QPixmap p, CardDirections dir ) = 0;

    /*!
     * \brief aseta kortti peliruudukolle
     *
     * Asettaa uuden kortin kuvan peliruudukolle ennettuihin
     * koortinaatteihin
     *
     * \param x kortin x-koordinaatti
     * \param y kortin y-koordinaatti
     * \param p laudalle tulevan kortin bittikarttakuva
     * \param dir asetettavan bittikarttakuvan kiertosuunto
     * \pre x,y osoittavat laudalla olevaan korttipaikkaan, ollaan pelitilassa
     * \post korttikuva näytetään käyttäjälle halutussa kohdassa peliruudukkoa
     */
    virtual void placeCard( int x, int y, QPixmap p, CardDirections dir ) = 0;

};

} // namespace

#endif // IGAMEVIEW_HH
