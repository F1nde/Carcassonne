#include "pointcard.hh"

pointCard::pointCard(QPixmap pic, std::deque<bool> links, QColor col) :
    Card( pic, links ), tokenColor_(col)
{

}

pointCard::~pointCard()
{

}

QColor pointCard::placeToken()
{
    // Haetaan kuva kantaluokan private-puolelta
    auto pic = getPicture();

    QPainter painter(&pic);
    painter.setRenderHint( QPainter::Antialiasing );
    QColor color = tokenColor_;
    painter.setBrush(color);

    // Keskikohta kovakoodattu ennen kuin keksitaan muu ratkaisu
    QPoint center( 16, 16 );
    painter.drawEllipse(center, 5, 5 );

    // Paivitetaan kuva kantaluokan private-osioon
    updatePicture(pic);

    // Palautetaan lisatyn nappulan vari pisteiden vahennysta varten
    return tokenColor_;

}
