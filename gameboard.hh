#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include <memory>
#include <vector>
#include <memory>

#include "card.hh"
#include "pointcard.hh"
#include "playerdatabase.hh"

/*!
 * \brief gameBoard-luokka.
 *
 *  Tama luokka pitaa sisallaan tietokannan pelilaudan tilanteesta. Aina kun
 *  laudalle asetetaan kortti, se lisataan taman luokan tietorakenteeseen. Lisaksi
 *  gameBoard -luokan vastuulla on laskea, syntyiko kortin asetuksen seurauksena
 *  uusia teita, asettaa tarvittavat pelinappulat ruudulle seka vahentaa oikea
 *  maara nappuloita pelaajilta.
 **/
class gameBoard
{
public:
    gameBoard( int width, int height, std::shared_ptr<playerDataBase> dataBase );
    ~gameBoard();

    /*!
     * \brief placeCard on metodi, joka luovuttaa pelilaudalle asetetun kortin pointterin pelilaudalle.
     * \param card on jaettu pointteri, joka osoittaa pelikorttiin, joka asetetaan pelilaudalle
     * \param x sisaltaa tiedon kortin tulevan sijainnin x-koordinaatista
     * \param y sisaltaa tiedon kortin tulevan sijainnin y-koordinaatista
     * \return paluuarvo (true/false) kertoo onnistuuko kortin asettaminen sijaintiin
     * \pre card != nullptr && 0 <= x-1 < gameBoard_.at(1).size() && 0 <= y-1 < gameBoard_.size()
     * \post annetun osoittimen paassa oleva kortti on asetettu annettuihin koordinaatteihin
     */
    bool placeCard( std::shared_ptr<Card> card, int x, int y );

    /*!
     * \brief checkRoads on metodi, joka kortin asettamisen jalkeen tarkistaa tuliko valmiiksi tie, joka on kummastakin paasta suljettu
     * \param x asetetun kortin x-koordinaatti
     * \param y asetetun kortin y-koordinaatti
     * \pre -
     * \post annettujen kordinaattejen paassa olevasta kortista on tarkistettu tuleeko teita valmiiksi. Teiden valmistuessa vahennetaan nappuloita kyseisilta pelaajilta
     */
    bool checkRoads( int x, int y, int direction );

    /*!
     * \brief palauttaa pelilaudan leveyden
     * \return pelilaudan leveys
     * \pre -
     */
    int getWidth();

    /*!
     * \brief palauttaa pelilaudan korkeuden
     * \return pelilaudan korkeus
     * \pre -
     */
    int getHeight();

    /*!
     * \brief palauttaa parametreina annettujen koordinaattien kohdalla olevan osoittimet
     * \param x kortin x-koordinaatti
     * \param y kortin y-koordinaatti
     * \pre 0 <= x < gameBoard_.at(1).size() && 0 <= y < gameBoard_.size()
     */
    std::shared_ptr<Card> getCardPointer( int x, int y );

private:

    struct tile_ {
        std::shared_ptr<Card> pointer_;
        bool lock_;
    };

    std::vector< std::vector< tile_ > > gameBoard_;
    std::vector< std::shared_ptr<pointCard> > pointcards_;
    bool firstTurn_;

    std::shared_ptr<playerDataBase> dataBase_;

    void cleaning();

    void checkPointcard( int x, int y );

    void placeTokens();

    bool cardAdjacent( int x, int y );

    /*!
     * \class gameBoard
     * \internal
     * \invariant pointcards_.size() == 0
     **/

    /*!
     * \brief invariantti testaa luokkainvariantin
     */
    void invariant() const;

};

#endif // GAMEBOARD_HH
