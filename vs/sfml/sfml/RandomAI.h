#pragma once
#include "I_AI.h"

struct BoardPos;


class RandomAI : public I_AI{
public:
    RandomAI();
    virtual const Meeple& selectOpponentsMeeple(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Board& board);
    virtual BoardPos selectMeeplePosition(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Board& board, const Meeple& meepleToSet);
};

