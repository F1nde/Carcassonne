#ifndef CARDSLOT_HH
#define CARDSLOT_HH

#include <functional>
#include <QGraphicsItem>

// tietotyyppi funktiolle, jota kutsutaan kun korttia tuplaklikataan.
typedef std::function<void(int,int)> checkF_t ;

/*
 * toteutamme oman verson QGraphicsItem-luokasta, jotta
 * oliomme muistaa itse asetetun kuvan ja pystyy myös
 * olemaan osana QGraphicsScene-rakennetta
 */
class CardSlot : public QGraphicsItem
{
public:
    /*!
     * CardSlot rakentaja
     * CardSlot on olio, joka kuvaa yhtä ruutua peliruudukossa.
     * Ruutuolio osaa piirtää itsensä (tyhjä neliö tai kortin grafiikka) ja
     * tuplaklikkauksella välittää tiedon ruudun valinnasta GameEngine-puolelle.
     *  x Tämän kortin x-koordinaatti
     *  y Kortin y-koordinaatti
     *  f Aliohjelma, jota kortti kutsuu kun sen alueelle on tuplaklikattu
     */
    explicit CardSlot(int x, int y, checkF_t f );
    CardSlot( const CardSlot& ) = delete;
    CardSlot operator=( const CardSlot& ) = delete;
    ~CardSlot() = default;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /* asetetaan piirrettävä kuva */
    void usePixmap( QPixmap );

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    int x_;
    int y_;
    checkF_t checkRoutine_;
    bool hasImage_;
    QPixmap myImage_;
};

#endif // CARDSLOT_HH
