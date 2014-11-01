#pragma once
#include <cstdint>

class Meeple;



struct BoardPos{
    uint8_t x;
    uint8_t y;
};


class BoardState
{
private:
    Meeple* board[4][4];
    
    
public:
    BoardState();
    virtual ~BoardState();

    Meeple* getMeeple(BoardPos position) const;
    bool isFieldEmpty(BoardPos position) const;
    void setMeeple(BoardPos position, Meeple& meeple);
};
