#include "Board.h"

#include <assert.h>
#include <iostream>
#include "Meeple.h"
#include "MeepleBag.h"

std::string BoardPos::toString() const{
    return std::string('(' + std::to_string(x + 1) + '|' + std::to_string(y + 1) + ')');
}

WinCombination::WinCombination(){
}

WinCombination::WinCombination(const WinCombination& base){
    for (int m = 0; m < 4; ++m){
        meeples[m] = nullptr;
        positions[m] = base.positions[m];
    }
}


Board::Board(){
    for (int y = 0; y < 4; ++y){
        for (int x = 0; x < 4; ++x){
            board[x][y] = nullptr;
        }
    }

    //populate the winCombinations-positions:

    //rows and columns:    
        for (uint8_t rc = 0; rc < 4; ++rc){     //Row or column (depending)
            WinCombination *row = new WinCombination();
            WinCombination *col = new WinCombination();

            for (uint8_t o = 0; o < 4; ++o){    //opposite axis
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
            
        for (uint8_t d = 0; d < 4; ++d){
            diaTL->positions[d] = { d, d };
            diaTR->positions[d] = { 3 - d, d };
            diaTL->meeples[d] = nullptr;          //no meeples set yet
            diaTR->meeples[d] = nullptr;          //no meeples set yet
            winCombinations.combination.insert(diaTL);
            winCombinations.combination.insert(diaTR);
        }
    isWinCombinationSetUp2Date = true;
}


Board::Board(const Board& base, const MeepleBag* bag1, const MeepleBag* bag2){
    for (int y = 0; y < 4; ++y){
        for (int x = 0; x < 4; ++x){
            if (base.board[x][y] == nullptr){
                board[x][y] = nullptr;
                continue;
            }
            const MeepleBag* correctBag = (bag1->getBagColor() == base.board[x][y]->getColor()) ? bag1 : bag2;      //Only search in the correct bag
            Meeple* correctMeeple = correctBag->getUsedMeepleRepresentation(*base.board[x][y]);
            assert(correctMeeple != nullptr);
            board[x][y] = correctMeeple;
        }
    }
    for (std::set<WinCombination*>::const_iterator it = base.winCombinations.combination.begin(); it != base.winCombinations.combination.end(); ++it){
       winCombinations.combination.insert(new WinCombination(**it));     //Make a copy   (meeplepointers are nullptr --> upToDate = false)
    }
    isWinCombinationSetUp2Date = false;
}




Board::~Board(){
    for (std::set<WinCombination*>::iterator it = winCombinations.combination.begin(); it != winCombinations.combination.end(); ++it){
        delete *it;
    }
    winCombinations.combination.clear();
}


void Board::reset(){
    for (int y = 0; y < 4; ++y){
        for (int x = 0; x < 4; ++x){
            board[x][y] = nullptr;
        }
    }
    isWinCombinationSetUp2Date = false;
}

const WinCombinationSet* Board::getWinCombinations() const{
    updateWinCombinations();
    return &winCombinations;
}

void Board::updateWinCombinations() const{
    if (isWinCombinationSetUp2Date){
        return;
    }
    for (std::set<WinCombination*>::iterator it = winCombinations.combination.begin(); it != winCombinations.combination.end(); ++it){
        for (int m = 0; m < 4; ++m){
            (*it)->meeples[m] = board[(*it)->positions[m].x][(*it)->positions[m].y];
        }
    }
    isWinCombinationSetUp2Date = true;
}

const Meeple* Board::getMeeple(BoardPos position) const{
    assert(position.x < 4 && position.y < 4);
    return board[position.x][position.y];
}

bool Board::isFieldEmpty(BoardPos position) const{
    assert(position.x < 4 && position.y < 4);
    return board[position.x][position.y] == nullptr;
}

void Board::setMeeple(BoardPos position, Meeple& meeple){
    assert(position.x < 4 && position.y < 4);
    assert(isFieldEmpty(position));
    board[position.x][position.y] = &meeple;
    isWinCombinationSetUp2Date = false;
}

Meeple* Board::removeMeeple(BoardPos position){
    Meeple* m = board[position.x][position.y];
    board[position.x][position.y] = nullptr;
    isWinCombinationSetUp2Date = false;
    return m;
}

bool Board::isFull() const{
    for (uint8_t y = 0; y < 4; ++y){
        for (uint8_t x = 0; x < 4; ++x){
            if (isFieldEmpty({ x, y })){
                return false;
            }
        }
    }
    return true;
}

BoardPos Board::getRandomEmptyField() const{
    BoardPos pos;
    do{
        pos.x = rand() % 4;
        pos.y = rand() % 4;
    } while (!isFieldEmpty(pos));
    return pos;
}





const WinCombination* Board::checkWinSituation() const{
    const WinCombinationSet* winCombinations = getWinCombinations();

    for (std::set<WinCombination*>::iterator it = winCombinations->combination.begin(); it != winCombinations->combination.end(); ++it){
        if (checkSimpleWinCombination(*it)){
            return *it;
        }
    }
    return nullptr;
}


bool Board::checkSimpleWinCombination(const WinCombination* comb) const{
    //check null-pointers:
        int m;
        for (m = 0; m < 4; ++m){
            if (comb->meeples[m] == nullptr){
                return false;
            }
        }
    //check similarity:
        for (int p = 0; p < 4; ++p){
            MeepleProperty prop = comb->meeples[0]->getProperty(p);
            bool match = true;
            for (m = 1; m < 4; ++m){
                if (comb->meeples[m]->getProperty(p) != prop){
                    match = false;
                    break;
                }
            }
            if (match){
                return true;
            }
        }
    return false;
}

void Board::print(std::ostream& output) const{
    //This function is just for debugging purposes.
    //Therefore it is allowed that it's ugly (and that it uses c-casts).
#pragma warning( push )
#pragma warning( disable: 4310)

    output << (char)201 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)187 << std::endl;
  

    for (uint8_t y = 0; y < 4; ++y){
        for (int l = 0; l < 2; ++l){
            output << (char)186 << ' ';
            for (uint8_t x = 0; x < 4; ++x){
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
        
#pragma warning( pop )

    return;
}

