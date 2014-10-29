#include "meeple.h"


Meeple::Meeple(bool isBig, bool isRound, bool hasHole){
    this->big = isBig;
    this->round = isRound;
    this->hole = hasHole;
}


bool Meeple::isBig() const{
    return big;
}

bool Meeple::isRound() const{
    return round;
}

bool Meeple::hasHole() const{
    return hole;
}