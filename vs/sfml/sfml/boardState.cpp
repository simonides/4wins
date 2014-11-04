#include "BoardState.h"

#include <assert.h>
#include <iostream>
#include "Meeple.h"


BoardState::BoardState(){
    for (int y = 0; y < 4; ++y){
        for (int x = 0; x < 4; ++x){
            board[x][y] = nullptr;
        }
    }

    //populate the winCombinations-positions:

    //rows and columns:    
        for (int rc = 0; rc < 4; ++rc){     //Row or column (depending)
            WinCombination *row = new WinCombination();
            WinCombination *col = new WinCombination();

            for (int o = 0; o < 4; ++o){    //opposite axis
                row->positions[o] = { o, rc };
                col->positions[o] = { rc, o };
                row->meeples[o] = nullptr;          //no meeples set yet
                col->meeples[o] = nullptr;          //no meeples set yet
                winCombinations.combination.insert(row);
                winCombinations.combination.insert(col);
            }
        }
    //diagonals:
        WinCombination *diaTL = new WinCombination();   //diagonal top-left to bottom-right
        WinCombination *diaTR = new WinCombination();   //diagonal top-right to bottom-left
            
        for (int d = 0; d < 4; ++d){
            diaTL->positions[d] = { d, d };
            diaTR->positions[d] = { 3 - d, d };
            diaTL->meeples[d] = nullptr;          //no meeples set yet
            diaTR->meeples[d] = nullptr;          //no meeples set yet
            winCombinations.combination.insert(diaTL);
            winCombinations.combination.insert(diaTR);
        }
    isWinCombinationSetUp2Date = true;
}


BoardState::~BoardState(){
    for (int y = 0; y < 4; ++y){
        for (int x = 0; x < 4; ++x){
            delete board[x][y];
        }
    }
    for (std::set<WinCombination*>::iterator it = winCombinations.combination.begin(); it != winCombinations.combination.end(); ++it){
        delete *it;
    }
    winCombinations.combination.clear();
}

const WinCombinationSet* BoardState::getWinCombinations() const{
    updateWinCombinations();
    return &winCombinations;
}

void BoardState::updateWinCombinations() const{
    if (isWinCombinationSetUp2Date){
        return;
    }
    for (std::set<WinCombination*>::iterator it = winCombinations.combination.begin(); it != winCombinations.combination.end(); ++it){
        for (int m = 0; m < 4; ++m){
            (*it)->meeples[m] = board[(*it)->positions[m].x][(*it)->positions[m].y];
        }
    }
    //TODO: how to not use a c-cast here?
    (bool)isWinCombinationSetUp2Date = true;
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
    isWinCombinationSetUp2Date = false;
}

Meeple* BoardState::removeMeeple(BoardPos position){
    Meeple* m = board[position.x][position.y];
    board[position.x][position.y] = nullptr;
    isWinCombinationSetUp2Date = false;
    return m;
}

bool BoardState::isFull() const{
    for (int y = 0; y < 4; ++y){
        for (int x = 0; x < 4; ++x){
            if (isFieldEmpty({ x, y })){
                return false;
            }
        }
    }
    return true;
}


const WinCombination* BoardState::checkWinSituation() const{
    const WinCombinationSet* winCombinations = getWinCombinations();

    for (std::set<WinCombination*>::iterator it = winCombinations->combination.begin(); it != winCombinations->combination.end(); ++it){
        if (checkSimpleWinCombination(*it)){
            return *it;
        }
    }
    return nullptr;
}


bool BoardState::checkSimpleWinCombination(const WinCombination* comb) const{
    int m;
    //check null-pointers:
        for (m = 0; m < 4; ++m){
            if (comb->meeples[m] == nullptr){
                return false;
            }
        }
    //check similarity:
        int color = comb->meeples[0]->getColor();
        int size = comb->meeples[0]->getSize();
        int shape = comb->meeples[0]->getShape();
        int detail = comb->meeples[0]->getDetail();        
        for (m = 1; m < 4; ++m){
            if (comb->meeples[m]->getColor() != color){
                color = -1;
            }
            if (comb->meeples[m]->getSize() != size){
                size = -1;
            }
            if (comb->meeples[m]->getShape() != shape){
                shape = -1;
            }
            if (comb->meeples[m]->getDetail() != detail){
                detail = -1;
            }
        }
        if (color != -1 || size != -1 || shape != -1 || detail != -1){
            return true;
        }
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
