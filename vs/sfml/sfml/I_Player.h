#pragma once

class BoardState;
class MeepleBag;
class Meeple;
struct BoardPos;


class I_Player{
//private:
public:   
    virtual const Meeple& selectOpponentsMeeple(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board) = 0;
    virtual BoardPos selectMeeplePosition(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board, const Meeple& meepleToSet) = 0;

    void reset();                               //Reinitialises the object
};

