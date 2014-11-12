#pragma once
#include "I_AI.h"

struct BoardPos;


class RandomAI : public I_AI{
public:
    RandomAI();
    virtual const Meeple& selectOpponentsMeeple(const GameState& gameState);
    virtual BoardPos selectMeeplePosition(const GameState& gameState, const Meeple& meepleToSet);
};

