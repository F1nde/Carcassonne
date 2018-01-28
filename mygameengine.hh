
#include "card.hh"
#include "gameboard.hh"
#include "IGameEngine.hh"
#include "playerdatabase.hh"
#include "statusscreen.hh"

#include <deque>
#include <memory>
#include <QColor>
#include <QDialog>
#include <QObject>
#include <QPixmap>
#include <QString>
#include <random>
#include <vector>

namespace Ui {
class Config;  // ennakkoesittely config-käyttöliittymälle
class endScreen; // ennakkoesittely endScreenille

}

/*!
 * \brief myEngine-luokka
 *
 * Toteutus pelimoottoriluokalle, joka toteuttaa rajapinnan IGameEnginen kanssa.
 * Hallitsee myös endScreen-ikkunan rakentumista ja statusScreen-luokan ylläpitoa.
 */
class myEngine : public QObject, public TIE2400::IGameEngine
{
    Q_OBJECT
public:
    /*!
     * \brief myEngine-luokan rakentaja
     */
    explicit myEngine();
    /*!
     * \brief luokan purkaja, purkaa luokan
     */
    virtual ~myEngine();

    // toteutukset rajapinnan vaatimille operaatioille
    void setView(TIE2400::IGameView* );
    void startConfiguration(QWidget* );
    void turnLeft();
    void turnRight();
    void locationClicked(int x, int y);



private slots:
    /*!
     * \brief slot joka muuttaa pelaajien tietolaatikoiden määrää sen mukaan kuinka monta pelaajaa peliin on valittu conf-ikkunassa
     * \param position kertoo sliderin tämänhetkisen arvon
     */
    void on_playerAmountSlider_sliderMoved(int position);

    /*!
     * \brief pelaajan 1 muutettua pudotusvalikosta väriänsä tämä slot aktivoituu ja muuttaa pelaajan värin tietokantaan pickedColors_
     * \param index kertoo pudotusvalikon valitun objektin järjestysnumeron
     */
    void player1colorPicked(int index);

    /*!
     * \brief pelaajan 2 muutettua pudotusvalikosta väriänsä tämä slot aktivoituu ja muuttaa pelaajan värin tietokantaan pickedColors_
     * \param index kertoo pudotusvalikon valitun objektin järjestysnumeron
     */
    void player2colorPicked(int index);

    /*!
     * \brief pelaajan 3 muutettua pudotusvalikosta väriänsä tämä slot aktivoituu ja muuttaa pelaajan värin tietokantaan pickedColors_
     * \param index kertoo pudotusvalikon valitun objektin järjestysnumeron
     */
    void player3colorPicked(int index);

    /*!
     * \brief pelaajan 4 muutettua pudotusvalikosta väriänsä tämä slot aktivoituu ja muuttaa pelaajan värin tietokantaan pickedColors_
     * \param index kertoo pudotusvalikon valitun objektin järjestysnumeron
     */
    void player4colorPicked(int index);

    /*!
     * \brief pelaajan 5 muutettua pudotusvalikosta väriänsä tämä slot aktivoituu ja muuttaa pelaajan värin tietokantaan pickedColors_
     * \param index kertoo pudotusvalikon valitun objektin järjestysnumeron
     */
    void player5colorPicked(int index);

    /*!
     * \brief pelaajan 6 muutettua pudotusvalikosta väriänsä tämä slot aktivoituu ja muuttaa pelaajan värin tietokantaan pickedColors_
     * \param index kertoo pudotusvalikon valitun objektin järjestysnumeron
     */
    void player6colorPicked(int index);

private:
    QDialog* dialog_;                                   //!< pointteri dialogiin jota käytetään confin pohjana
    Ui::Config* ui_dialog_;                             //!< pointteri itse config-ikkunaan
    statusScreen* ui_statusScreen_;                     //!< pointteri statusScreen-luokkaa eli tilastoikkunaa varten
    QDialog* endDialog_;                                //!< pointteri loppunäyttöä (tulokset) varten
    Ui::endScreen* ui_endscreen_;                       //!< pointteri lopetusikkunaan
    TIE2400::IGameView* view_;                          //!<

    std::vector<QPixmap> cardImages_;                   //!< tietorakenne joka pitää sisällään QPixmapit pelin korteista
    std::deque< std::shared_ptr<Card> > deck_;          //!< tietorakenne jossa säilötään Card-luokasta rakennettujen korttien pakkaa
    std::shared_ptr<Card> currentCard_;                 //!< jaettu pointteri pakasta nostettua korttia varten
    unsigned int cardRotation_;                         //!< lukuarvo jota käytetään kuvaamaan kortin kiertomäärää, määrittää kortin orientaation modulo4:n avulla

    std::unique_ptr<gameBoard> gameBoard_;              //!< uniikki pointteri gameBoard-luokkaa eli pelilautaa varten
    std::shared_ptr<playerDataBase> dataBase_;          //!< jaettu pointteri playerDataBase-luokkaan eli pelaajien tietokantaa varten


    std::random_device rd_;                             //!<
    std::default_random_engine re_;                     //!<

    /*!
     * \brief initializeDeck
     * \param p
     * \param exp
     */
    void initializeDeck( int p, bool exp );
    TIE2400::CardDirections getDirection( unsigned int r );

    /*!
     * \brief pelilaudan päivittävä metodi
     */
    void updateUIboard();

    /*!
     * \brief metodi, joka pelin loputtua tuo näkyviin ikkunan pelin voittaneista pelaajista
     */
    void showEndScreen();
    
    /*!
     * \brief funktio joka harmaantaa OK-nappulan jos asetukset ei ole kunnossa
     */
    void configOkStatus();

    std::vector<int> pickedColors_;                     //!< tietorakenne joka sisältää pelaajien valitsemien värien int-indeksejä gameConstants playerColors-vektoriin
    std::vector<QColor> pickedQColors_;                 //!< tietorakenne joka sisältää pelaajien valitsemat värit QColoreina
    std::vector<QString> pickedNames_;                  //!< tietorakenne joka sisältää pelaajien nimet QStringeinä
};
