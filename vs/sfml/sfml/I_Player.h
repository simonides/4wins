#pragma once

class Board;
class MeepleBag;
class Meeple;
struct BoardPos;


class I_Player{
public:   
    virtual const Meeple& selectOpponentsMeeple(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Board& board) = 0;
    virtual BoardPos selectMeeplePosition(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Board& board, const Meeple& meepleToSet) = 0;

    void reset();                               //Reinitialises the object
};

