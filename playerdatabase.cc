#include "playerdatabase.hh"
#include "gameconstants.hh"

#include <string>
#include <sstream>
#include <QMap>
#include <QColor>
#include <QString>
#include <QDebug>

playerDataBase::playerDataBase( int players ): tokens_()
{
    Q_ASSERT( 0 < players && players < 7 );
    currentPlayer_ = rand() % players + 1;
    maxPlayers_ = players;
    Q_ASSERT( maxPlayers_ == static_cast<size_t>( players ) );
}

playerDataBase::~playerDataBase()
{
}

void playerDataBase::addPlayer( QColor color )
{
    tokens_[color.name()] = 6;
    Q_ASSERT( tokens_.size() <= static_cast<signed int>( maxPlayers_ ) );
    return;
}

std::string playerDataBase::switchPlayer()
{
    currentPlayer_ += 1;

    if( currentPlayer_ > maxPlayers_ )
    {
        currentPlayer_ = 1;
    }

    std::stringstream temp;
    temp << currentPlayer_;
    std::string player = temp.str();
    invariant();

    return player;
}

int playerDataBase::getTokens( QColor color )
{
    Q_ASSERT( tokens_.find(color.name()) != tokens_.end() );
    return tokens_[color.name()];
}

bool playerDataBase::checkIfEnds()
{
    for( QMap< QString, int>::iterator i = tokens_.begin(); i!=tokens_.end(); ++i )
    {
        if( i.value() == 0 ){
            qDebug() << "Nappulat loppuivat.";
            return true;
        }
    }

    return false;
}

void playerDataBase::changeTokens( QColor color )
{
    Q_ASSERT( tokens_.find(color.name()) != tokens_.end() );
    if( tokens_[color.name()] != 0){
        tokens_[color.name()] -= 1;
    }
    return;
}

void playerDataBase::invariant() const
{
    Q_ASSERT( 0 < currentPlayer_ && currentPlayer_ <= maxPlayers_ );
    return;
}
