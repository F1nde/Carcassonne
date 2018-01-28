#ifndef STATUSSCREEN_HH
#define STATUSSCREEN_HH

#include <QLCDNumber>
#include <QString>
#include <QWidget>
#include <vector>


namespace Ui {
class statusScreen;
}

/*!
 * \brief statusScreen-luokka
 *
 * Luokka joka hallitsee lisäosana peliin toteutettua tilanneikkunaa.
 * Sen julkisen rajapinnan kautta voidaan muuttaa ja kysyä pelaajien pistemääriä ja kertoa kun kortti pelataan pöytään, jotta korttilaskuri päivittyy.
 **/
class statusScreen : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief Tilanneikkunan rakentaja
     * \param tietorakenne, joka sisältää pelaajien nimet QStringeinä
     * \param kokonaisluku pelin pelaajamäärästä
     * \param tietorakenne, joka sisältää pelaajien valitsemat värit QColoreina
     * \param kokonaisluku kortin koosta
     * \param -
     */
    explicit statusScreen(std::vector<QString> playerNames, int playerAmount, std::vector<QColor> playerColors, int deckSize, QWidget *parent = 0);

    /*!
     * \brief purkaja luokalle
     */
    ~statusScreen();

    /*!
     * \brief metodi, joka asettaa halutun pistemäärän halutulle pelaajalle
     * \param kohdepelaaja, jonka pistemäärää halutaan muuttaa
     * \param uusi pistemäärä
     */
    void setPoints(int targetPlayer, int amount);

    /*!
     * \brief metodi, joka kertoo että kortti on pelattu -> korttilaskuria kasvatetaan
     */
    void updateCardAmount();

    /*!
     * \brief metodi jolla luokka kertoo halkutun pelaajan pistemäärän
     * \param pelaaja jonka pistemäärä kerrotaan
     * \return
     */
    int getPoints(int targetPlayer);

private:
    Ui::statusScreen *ui;                           //!< pointteri UI:hin
    std::vector<QLCDNumber*> playerPointLabels_;    //!< tietorakenne, joka sisältää pointterit pelaajien pistemääriä hallitseviin objekteihin (QLCDNumber)
    int cardAmount_;                                //!< kokonaisluku joka kertoo tällä hetkellä pelattujen korttien määrän
    int deckSize_;                                  //!< kokonaisluku joka kertoo koko pakan kokonaismäärän

    /*!
     * \brief luokkainvariantin testaaja
     */
    void invariant() const;
};

#endif // STATUSSCREEN_HH
