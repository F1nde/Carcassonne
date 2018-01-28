#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include <string>

namespace TIE2400 {

// kortin mahdolliset asennot
enum class CardDirections { N, E, S, W };

//  montako pikseliä on kortin kuvaneliön sivu
const int CARD_SIZE = 32;
const int CARD_HALF_SIZE = CARD_SIZE / 2;

// montako pikseliä jätetää tyhjää tilaa
const int CARD_MARGIN = 0;

// kuinka leveällä "kynällä" piirretään peliruudun neliö
const int CARD_SLOT_PEN = 1;

// | margin | line | margin | card_image | margin | line | margin |
const int CARD_TOTAL_LEN = 2*(2*CARD_MARGIN + CARD_SLOT_PEN) + CARD_SIZE;

const std::string DATADIR("gamedata");


} // namespace

#endif // CONSTANTS_HH
