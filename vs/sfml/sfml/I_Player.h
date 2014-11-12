#pragma once

struct GameState;
class Board;
class MeepleBag;
class Meeple;
struct BoardPos;


class I_Player{
public:
    virtual const Meeple& selectOpponentsMeeple(const GameState& gameState) = 0;
    virtual BoardPos selectMeeplePosition(const GameState& gameState, const Meeple& meepleToSet) = 0;
    
    void reset();                               //Reinitialises the object for a new game
};

