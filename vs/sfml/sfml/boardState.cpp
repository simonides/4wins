#include "BoardState.h"
#include <assert.h>

BoardState::BoardState(){
    for (int y = 0; y < 4; y++){
        for (int x = 0; x < 4; x++){
            board[x][y] = nullptr;
        }
    }
}


BoardState::~BoardState(){
}



Meeple* BoardState::getMeeple(BoardPos position) const{
    assert(position.x < 4 && position.y < 4);
    return board[position.x][position.y];
}

bool BoardState::isFieldEmpty(BoardPos position) const{
    assert(position.x < 4 && position.y < 4);
    return board[position.x][position.y] == nullptr;
}

void BoardState::setMeeple(BoardPos position, Meeple& meeple){
    assert(position.x < 4 && position.y < 4);
    assert(isFieldEmpty(position.x, position.y));
    board[position.x][position.y] = &meeple;
}