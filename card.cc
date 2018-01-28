#include "card.hh"
#include <vector>
#include <QDebug>

Card::Card()
{

}

Card::Card(QPixmap pic, std::deque<bool> links) :
    picture_(pic), linking_(links), rotation_(0)
{
}

Card::~Card()
{
}

void Card::turn( unsigned int i )
{
    Q_ASSERT( i <= 3 ); // Esiehto
    // Paivitetaan kortin tieto sen pyorimisesta
    rotation_ = i;

    bool temp;
    // Pyoritetaan linkkitietokantaa
    for( unsigned int j = 0; j < i; ++j ) {
        temp = linking_.back();
        linking_.pop_back();
        linking_.push_front(temp);
    }

    Q_ASSERT( rotation_ == i );
    invariant();

}

std::vector<bool> Card::link()
{
    // Muutetaan deque-rakenne vector-rakenteeksi pelilautaa varten
    std::vector<bool> links;
    for( size_t i = 0; i < linking_.size(); ++i ) {
        links.push_back( linking_.at(i) );
    }

    invariant();
    return links;
}

QPixmap Card::getPicture()
{
    return picture_;
}

unsigned int Card::getRotation()
{
    return rotation_;
}

void Card::invariant() const
{
    Q_ASSERT( rotation_ <= 3 && linking_.size() == 4 );
}

void Card::updatePicture( QPixmap pic )
{
    picture_ = pic;
}



