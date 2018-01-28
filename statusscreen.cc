#include "ui_statusscreen.h"

#include "statusscreen.hh"

#include <QDebug>
#include <QLabel>
#include <QLCDNumber>
#include <QPalette>
#include <vector>

statusScreen::statusScreen(std::vector<QString> playerNames, int playerAmount, std::vector<QColor> playerColors, int deckSize, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::statusScreen), playerPointLabels_(), cardAmount_(0), deckSize_(deckSize)
{
    ui->setupUi(this);

    // Muutetaan otsikko
    this->setWindowTitle("Status Screen");

    // Rakennetaan vektori jossa on pointterit pelaajien pistemäärien ruutuihin
    playerPointLabels_ = {ui->player1Points,ui->player2Points,ui->player3Points,ui->player4Points,ui->player5Points,ui->player6Points};

    // Rakennetaan pelaajien pistemäärien taulukko
    // Tyhjät pelaajapaikat piilotetaan, käytettyihin laitetaan nimet kohdilleen
    std::vector<QLabel*> playerNameLabels {ui->player1Name,ui->player2Name,ui->player3Name,ui->player4Name,ui->player5Name,ui->player6Name};
    std::vector<QLabel*> playerColorLabels {ui->player1Color,ui->player2Color,ui->player3Color,ui->player4Color,ui->player5Color,ui->player6Color};

    QLabel* tempName = nullptr;
    QLabel* tempColor = nullptr;
    QLCDNumber* tempPoint = nullptr;
    for(int stepper = 0; stepper < 6; ++stepper)
    {
        tempName = playerNameLabels.at(stepper);
        tempColor = playerColorLabels.at(stepper);
        tempPoint = playerPointLabels_.at(stepper);
        if(stepper >= playerAmount)
        {
            qDebug() << "Piilotetaan pelaajan " << stepper+1 << " nimi";
            tempName->setHidden(true);
            tempColor->setHidden(true);
            tempPoint->setHidden(true);
        }
        else
        {
            tempName->setText(playerNames.at(stepper));

            // Värin säätö
            QPalette temp;
            temp.setColor(QPalette::Window, playerColors.at(stepper));
            tempColor->setAutoFillBackground(true);
            tempColor->setPalette(temp);
            // alkupisteet
            tempPoint->display(6);
        }
    }

}

statusScreen::~statusScreen()
{
    delete ui;
}

void statusScreen::setPoints(int targetPlayer, int amount)
{
    // Lisätään piste kohdepelaajalle
    QLCDNumber* temp = nullptr;
    temp = playerPointLabels_.at(targetPlayer-1);
    temp->display(amount);
}

void statusScreen::updateCardAmount()
{
    ++cardAmount_;
    ui->cardsUsedBar->setValue((cardAmount_*100)/deckSize_);
}

int statusScreen::getPoints(int targetPlayer)
{
    std::vector<QLCDNumber*> lcdNumbers {ui->player1Points,ui->player2Points,ui->player3Points,ui->player4Points,ui->player5Points,ui->player6Points};
    QLCDNumber* temp = lcdNumbers.at(targetPlayer-1);
    return temp->intValue();
}

void statusScreen::invariant() const
{
    Q_ASSERT( 0 <= cardAmount_ && cardAmount_ <= deckSize_ );
    return;
}
