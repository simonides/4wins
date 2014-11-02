#include "BoardState.h"

#include <assert.h>
#include <iostream>
#include "Meeple.h"


BoardState::BoardState(){
    for (int y = 0; y < 4; y++){
        for (int x = 0; x < 4; x++){
            board[x][y] = nullptr;
        }
    }
}


BoardState::~BoardState(){
    for (int y = 0; y < 4; y++){
        for (int x = 0; x < 4; x++){
            delete board[x][y];
        }
    }
}



const Meeple* BoardState::getMeeple(BoardPos position) const{
    assert(position.x < 4 && position.y < 4);
    return board[position.x][position.y];
}

bool BoardState::isFieldEmpty(BoardPos position) const{
    assert(position.x < 4 && position.y < 4);
    return board[position.x][position.y] == nullptr;
}

void BoardState::setMeeple(BoardPos position, Meeple& meeple){
    assert(position.x < 4 && position.y < 4);
    assert(isFieldEmpty(position));
    board[position.x][position.y] = &meeple;
}

bool BoardState::isFull() const{
    for (int y = 0; y < 4; y++){
        for (int x = 0; x < 4; x++){
            if (isFieldEmpty({ x, y })){
                return false;
            }
        }
    }
    return true;
}


bool BoardState::checkWinSituation() const{  
    const Meeple *row[4];
    const Meeple *col[4];

    //Check rows and columns:    
        for (int rc = 0; rc < 4; rc++){     //Row or column (depending)
            for (int o = 0; o < 4; o++){    //opposite axis
                row[o] = board[o][rc];      
                col[o] = board[rc][o];
            }
            if (checkSimpleWinSituation(row) || checkSimpleWinSituation(col)){
                return true;
            }
        }
    //Check diagonals:
        const Meeple *diaTL[4];     //diagonal top-left to bottom-right
        const Meeple *diaTR[4];     //diagonal top-right to bottom-left
        for (int d = 0; d < 4; d++){
            diaTL[d] = board[d][d];
            diaTR[d] = board[3-d][d];
        }
        if (checkSimpleWinSituation(diaTL) || checkSimpleWinSituation(diaTR)){
            return true;
        }

    return false;
}


bool BoardState::checkSimpleWinSituation(const Meeple *meeple[4]) const{
    int m;
    //check null-pointers:
        for (m = 0; m < 4; m++){
            if (meeple[m] == nullptr){
                return false;
            }
        }
    
    //color:
        MeepleColor::Enum color = meeple[0]->getColor();
        for (m = 1; m < 4; m++){
            if (meeple[m]->getColor() != color){    //no match
                goto check_size;
            }
        }
        return true;
    check_size:
        MeepleSize::Enum size = meeple[0]->getSize();
        for (m = 1; m < 4; m++){
            if (meeple[m]->getSize() != size){    //no match
                goto check_shape;
            }
        }
        return true;
    check_shape:
        MeepleShape::Enum shape = meeple[0]->getShape();
        for (m = 1; m < 4; m++){
            if (meeple[m]->getShape() != shape){    //no match
                goto check_detail;
            }
        }
        return true;
    check_detail:
        MeepleDetail::Enum detail = meeple[0]->getDetail();
        for (m = 1; m < 4; m++){
            if (meeple[m]->getDetail() != detail){    //no match
                goto nomatch;
            }
        }
        return true;
    
    nomatch:
        return false;
}





void BoardState::print(std::ostream& output) const{
    //This function is just for debugging purposes.
    //It is allowed, that this function is ugly, and that it uses c-casts.
    
    output << (char)201 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)187 << std::endl;
        
    for (int y = 0; y < 4; y++){
        for (int l = 0; l < 2; l++){
            output << (char)186 << ' ';
            for (int x = 0; x < 4; x++){
                if (isFieldEmpty({ x, y })){
                    output << "   ";
                    continue;
                }
                const Meeple* m = getMeeple({ x, y });

                if (l == 0){
                    output << ((m->getColor() == MeepleColor::WHITE) ? (char)2/*178*/ : (char)1/*176*/);
                    output << ((m->getSize() == MeepleSize::BIG) ? '+' : '-');
                }
                else{
                    output << ((m->getShape() == MeepleShape::SQUARE) ? (char)254 : 'o');
                    output << ((m->getDetail() == MeepleDetail::HOLE) ? (char)155 : (char)207);
                }
                output << ' ';
            }
            output << (char)186 << std::endl;
        }
        output << (char)186 << "             " << (char)186 << std::endl;
    }

    output << (char)200 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)188 << std::endl;
        
    return;
}