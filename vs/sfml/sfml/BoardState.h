#pragma once

#include <assert.h>

class Meeple;

class BoardState
{
private:
    Meeple* board[4][4];



    
public:
    BoardState();
    virtual ~BoardState();

    Meeple* getMeeple(unsigned int x, unsigned int y) const;
    bool isFieldEmpty(unsigned int x, unsigned int y) const;
    void setMeeple(unsigned int x, unsigned int y, Meeple& meeple);
};
