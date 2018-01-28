#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "IGameView.hh"
#include "constants.hh"
#include "cardslot.hh"
#include <map>

namespace Ui {
class MainWindow;
}

class GameUI : public QMainWindow, TIE2400::IGameView
{
    Q_OBJECT

public:
    explicit GameUI( QWidget *parent = 0 );
    ~GameUI();

    // toteutukset IGameView rajapinnalle
    void setStatus(std::string message, unsigned int time );
    virtual void setGameMode( void );
    virtual void showCard(const QPixmap, TIE2400::CardDirections dir );
    virtual void placeCard( int x, int y, QPixmap p, TIE2400::CardDirections dir );


public Q_SLOTS:
    void start();
    void rotateLeft();
    void rotateRight();

private:

    Ui::MainWindow *ui;
    TIE2400::IGameView* view_;
    std::map< std::tuple<int,int>, CardSlot* > slots_;

    void populateSlots(QGraphicsView *v);
    void doubleClickCard(int,int);
    QPixmap rotate(const QPixmap &, TIE2400::CardDirections );

};

#endif // MAINWINDOW_H
