#pragma once
#include <cstdint>
#include <string>

class Meeple;



struct BoardPos{
    uint8_t x;
    uint8_t y;
};


class BoardState
{
private:
    const Meeple* board[4][4];
    

    bool checkSimpleWinSituation(const Meeple *m[4]) const;       //Checks 4 meeples for similarity
public:
    BoardState();
    virtual ~BoardState();

    const Meeple* getMeeple(BoardPos position) const;
    bool isFieldEmpty(BoardPos position) const;
    void setMeeple(BoardPos position, Meeple& meeple);

    bool isFull() const;
    
    bool checkWinSituation() const;     //Checks, if there are 4 similar meeples in a row/col/diagonal
    
    void print(std::ostream& output) const; //For debugging purposes only
};
