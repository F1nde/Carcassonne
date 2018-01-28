#ifndef POINTCARD_HH
#define POINTCARD_HH

#include <QPainter>
#include "card.hh"


/*!
 * \brief pointCard-luokka.
 *
 *  Tama luokka pitaa sisallaan yhteen pistekorttiin sisaltyvan informaation
 *  eli tiedon kortin kuvasta, kierrosmaarasta, kortissa olevien sivujen
 *  yhteyksista seka kortissa mahdollisesti olevan nappulan varista.
 **/
class pointCard : public Card
{
public:
    /*!
    * \brief Rakentaja kortin, jossa on annettu kuva, linkkitietorakenne ja tieto kortin varista
    * \param pic Korttiin asetettava QPixmap kuva
    * \param links Kortin reiteista kertova linkkitietorakenne
    * \param col Korttiin asetettavan nappulan vari
    * \pre links.size() == 4
    **/
    pointCard( QPixmap pic, std::deque<bool> links, QColor col );

    /*!
    * \brief Purkaja purkaa pistekortin
    * \pre -
    **/
    ~pointCard();

    /*!
     * \brief placeToken asettaa pelaajan nappulan ruutuun
     * \return palauttaa lisatyn nappulan varin QColor -tyyppisena nappuloiden vahentamista varten
     * \pre -
     * \post kortin kuvaan on asetettu oikean varinen nappula
     */
    QColor placeToken();

private:
    QColor tokenColor_;         //!< Kortin nappulan ja nappulapaikan vari
};

#endif // POINTCARD_HH
