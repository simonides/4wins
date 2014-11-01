#pragma once
#include "I_AI.h"

struct BoardPos;


class RandomAI : public I_AI
{
private:

public:
    RandomAI();
    virtual ~RandomAI();

    virtual const Meeple& selectOpponentsMeeple(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board);
    virtual BoardPos selectMeeplePosition(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board, const Meeple& meepleToSet);
};

