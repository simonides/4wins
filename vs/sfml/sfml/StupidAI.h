#pragma once
#include "I_AI.h"

//This AI always choses the first meeple in the bag, and puts meeples at the first free position on the field
class StupidAI : public I_AI{
public:
    virtual const Meeple& selectOpponentsMeeple(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Board& board);
    virtual BoardPos selectMeeplePosition(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Board& board, const Meeple& meepleToSet);
};

