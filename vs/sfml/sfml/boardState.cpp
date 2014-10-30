#include "BoardState.h"


BoardState::BoardState(){
    for (int y = 0; y < 4; y++){
        for (int x = 0; x < 4; x++){
            board[x][y] = nullptr;
        }
    }
}


BoardState::~BoardState(){
}



Meeple* BoardState::getMeeple(unsigned int x, unsigned int y) const{
    assert(x<4 && y<4);
    return board[x][y];
}

bool BoardState::isFieldEmpty(unsigned int x, unsigned int y) const{
    assert(x<4 && y<4);
    return board[x][y] == nullptr;
}

void BoardState::setMeeple(unsigned int x, unsigned int y, Meeple& meeple){
    assert(x<4 && y<4);
    assert(isFieldEmpty(x, y));
    board[x][y] = &meeple;
}