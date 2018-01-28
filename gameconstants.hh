#ifndef GAMECONSTANTS_HH
#define GAMECONSTANTS_HH

#include <QPainter>
#include <vector>
#include <deque>
#include <utility>

namespace gameConstants {

// mahdolliset suunnat, joihin kortista voidaan menna
enum CardLinks { UP, RIGHT, DOWN, LEFT, DE };

// erilaisten korttityyppien lukumaarat
const int CARD_THROUGH = 4;
const int CARD_CURVE = 4;
const int CARD_DE = 5;
const int CARD_EXP = 1;
const int CARD_THROUGH_T = 5;
const int CARD_CURVE_T = 5;

// Erilaisten korttityyppien linkkitiedot
const std::deque<bool> L_THROUGH = {true, false, true, false};
const std::deque<bool> L_CURVE = {false, false, true, true};
const std::deque<bool> L_DE = {false, false, false, false};
const std::deque<bool> L_EXP_CROSS = {true, true, true, true};
const std::deque<bool> L_EXP_TCROSS = {true, true, false, true};

// Mahdolliset nappuloiden varit
const std::vector<QColor> playerColors = {Qt::red, Qt::black, Qt::blue, Qt::gray, Qt::magenta, Qt::cyan};
const std::vector<QString> playerColorNames = {"Red","Black","Blue","Gray","Magenta","Cyan"};

// Kortteihin piirrettavien nappulapaikkojen reunojen leveys
const int tokenAreaWidth = 3;

// tie algoritmi: tulo suuntaa vastaavat suunnat ( pohjoinen, ita, lansi, etela )
const std::vector<enum CardLinks> comingDirection = { {DOWN},{LEFT},{UP},{RIGHT} };

// tie algoritmi: xy muutokset rekursio kutsuja varten( pohjoinen, ita, lansi, etela )
const std::vector< std::pair<int,int> > compass = { { 0,-1 },{ 1,0 },{ 0,1 },{ -1,0 } };

} //namespace


#endif // GAMECONSTANTS_HH
