#include "cardslot.hh"
#include "constants.hh"

#include <QPainter>
#include <QDebug>

// rakentaja. Talletetaan parametrin olion jäsenmuuttujiin
CardSlot::CardSlot(int x, int y, checkF_t f ) :
    x_(x), y_(y), checkRoutine_(f), hasImage_(false), myImage_()
{}

// Qt-kirjasto kysyy tällä kutsulla mikä on piirtoalueemme koko
QRectF CardSlot::boundingRect() const
{
    using namespace TIE2400;
    return QRectF( 0,0, CARD_TOTAL_LEN-1, CARD_TOTAL_LEN-1);
}

// Qt-kirjasto kutsuu tätä kun oliotamme vastaava grafiikka-alue pitää piirtää
void CardSlot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    using namespace TIE2400;
    Q_UNUSED( widget );
    Q_UNUSED( option );

    QColor penColor( "lightgrey" );
    QPen pen;
    pen.setWidth( CARD_SLOT_PEN );
    pen.setColor( penColor );
    painter->setPen( pen );

    // piirretään aina korttipaikan kohdalle neliö
    painter->drawRect( CARD_MARGIN, CARD_MARGIN, CARD_SIZE+1, CARD_SIZE+1 );

    if(not hasImage_) {
        // Jos ruudussa ei ole vielä kuvaa, niin piirretään
        // tyhjän neliön sisään kohdan koordinaatit tekstinä
        auto font = painter->font();
        font.setPointSize( 6 );
        painter->setFont( font );
        int xy = CARD_MARGIN*2 + CARD_SLOT_PEN + 2;
        painter->drawText(xy, xy+8, QString("%1,%2").arg(x_).arg(y_));
    } else {
        // meillä on olemassa kuva, joten se piirretään neliön sisälle
        painter->drawPixmap(CARD_MARGIN+CARD_SLOT_PEN, CARD_MARGIN+CARD_SLOT_PEN,
                            CARD_SIZE, CARD_SIZE,
                            myImage_ );
    }
}

void CardSlot::usePixmap(QPixmap p )
{
    hasImage_ = true;
    myImage_ = p;
    this->update();
}

// Qt kutsuu tätä kun oliota vastaavalle kohdalle GraphicsScene:ssä
// tulee tuplaklikkaus
void CardSlot::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    // ilmoitetaan ylemmäs (näkymälle) klikkauksesta
    checkRoutine_(x_,y_);

    // pyydetään varmuuden vuoksi päivitys kuvalle
    // jos edellisen kutsun seurauksena olisi tullut muutoksia
    this->update();
}
