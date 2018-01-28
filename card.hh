#ifndef CARD_HH
#define CARD_HH

#include <QPixmap>
#include <vector>
#include <deque>
#include "gameconstants.hh"

/*!
 * \brief Card-luokka.
 *
 *  Tama luokka pitaa sisallaan yhteen korttiin sisaltyvan informaation
 *  eli tiedon kortin kuvasta, kierrosmaarasta, ja kortissa olevien sivujen
 *  yhteyksista. Huomioitavaa on, etta korteille, joihin nappula voidaan asettaa,
 *  on oma luokka, joka on periytetty tasta luokasta.
 **/
class Card
{
public:
    Card();

    /*!
    * \brief Rakentaa kortin, jossa on annettu kuva ja linkkitietorakenne
    * \param pic Korttiin asetettava QPixmap kuva
    * \param links Kortin reiteista kertova linkkitietorakenne
    * \pre links.size() == 4
    **/
    Card( QPixmap pic, std::deque<bool> links );

    /*!
    * \brief Purkaja purkaa kortin
    * \pre -
    **/
    virtual ~Card();

    /*!
     * \brief kaannetaan kortin linkkitietoja annetun kokonaisluvun verran
     * \param i maara, jolla linkkitietokantaa tulee kaantaa
     * \pre i <= 3
     * \post Kortin kierrosluku on asetettu annetuksi ja linkkitietorakennetta on pyoritetty sen verran
     */
    void turn( unsigned int i );

    /*!
     * \brief palauttaa kortin linkkitietokannan vector<bool> -tyyppisena
     * \return palauttaa kortin linkkitietokannan
     * \pre -
     */
    std::vector<bool> link();

    /*!
     * \brief palauttaa kortin kuvan, kuva on aina alkuperaisessa muodossaan eli ei pyoritettyna
     * \return kortissa oleva kuva
     * \pre -
     */
    QPixmap getPicture();

    /*!
     * \brief palauttaa tiedon siita, kuinka paljon korttia on pyoritetty
     * \return kortin kiertymismaara positiivisena kokonaislukuna
     * \pre -
     */
    unsigned int getRotation();

private:
    QPixmap picture_;               //!< Kortin kuva
    std::deque<bool> linking_;      //!< Kortin reiteista kertova linkkitietorakenne
    unsigned int rotation_;         //!< Kortin kierrosluvun kertova kokonaisluku

    /*!
     * \class Card
     * \internal
     * \invariant rotation_ <= 3 && linking_.size() == 4
     **/

    /*!
     * \brief invariantti testaa luokkainvariantin
     */
    void invariant() const;

protected:
    /*!
     * \brief asettaa parametrina annetun kuvan kortin kuvaksi
     * \param pic Korttiin asetettava kuva
     * \pre -
     * \post parametrina annettu kuva on asetettu kortin kuvaksi
     */
    void updatePicture( QPixmap pic );
};

#endif // CARD_HH
