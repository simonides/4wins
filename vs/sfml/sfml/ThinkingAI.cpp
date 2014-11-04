#include "ThinkingAI.h"

#include "BoardState.h"
#include "MeepleBag.h"
#include "config.h"

#include <iostream>


ThinkingAI::ThinkingAI(bool pretendStupidity) : pretendStupidity(pretendStupidity){
}

const Meeple& ThinkingAI::selectOpponentsMeeple(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& originalBoard){
    return **(opponentBag.getMeeples()->cbegin()); 
    
    /*const std::set<const Meeple*>* meeples = opponentBag.getMeeples();

    BoardState* board = new BoardState(originalBoard);  //Make a copy for temporary checks
    
    for (std::set<const Meeple*>::const_iterator it = meeples->begin(); it != meeples->end(); ++it){
        for (int y = 0; y < 4; ++y){
            for (int x = 0; x < 4; ++x){
                if (board->isFieldEmpty({ x, y })){     //Check, if the opponent wins, if we choose this meeple
                    //board->setMeeple()
                }
            }
        } 
    }
    //todo: copy-constructor vom board überschreiben, damit die spielfiguren nicht doppelt freigegeben werden
    //ODER: im MEEPLEBAG 2 listen speichern, eine mit used, eine mit unused, und dort alles freigeben!
    delete board;
    throw new std::exception("No meeples!");*/
}


BoardPos ThinkingAI::selectMeeplePosition(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board, const Meeple& meepleToSet){
    unsigned int scoreMap[4][4] = { 0 };

    //go through the whole map, and add points if the meeple should be set there
    //  --> check all possible combinations, which can lead to a victory
    //  --> look at all meeples, that are already placed in the combination
    //  --> check, if these meeples have any similarities with the meepleToSet. Calculate matching-points
    //  --> add the matching-points to all fields in the combination, where no meeple is currently placed
    //Afterwards, go through the scoreMap, and choose the field with the most points

    const WinCombinationSet* allCombinations = board.getWinCombinations();
    for (std::set<WinCombination*>::const_iterator it = allCombinations->combination.begin(); it != allCombinations->combination.end(); it++){
        WinCombination* comb = *it;
        int points = getPointsForCombination(comb->meeples, meepleToSet);
        for (int m = 0; m < 4; m++){
            if (comb->meeples[m] == nullptr){    //The field is empty --> add the points
                scoreMap[comb->positions[m].x][comb->positions[m].y] += points;
            }
        }
    }
    
    //Get the field with the most points:
    unsigned int points = scoreMap[0][0];
    BoardPos pos = { 0, 0 };
    unsigned int randMod = 2;

    if (PRINT_THINK_MAP){
        std::cout << "Thinkingmap:" << std::endl;
    }
    for (unsigned int y = 0; y < 4; y++){
        for (unsigned int x = 0; x < 4; x++){
            if (PRINT_THINK_MAP){
                std::cout << scoreMap[x][y];
            }
            if (scoreMap[x][y] > points || (scoreMap[x][y] == points && (rand() % (randMod++)) == 0)){
                points = scoreMap[x][y];
                pos = { x, y };
                if (PRINT_THINK_MAP){
                    std::cout << "! ";
                }
            }else{
                if (PRINT_THINK_MAP){
                    std::cout << "  ";
                }
            }
        }
        if (PRINT_THINK_MAP){
            std::cout << std::endl;
        }
    }
    if (PRINT_THINK_MAP){
        std::cout << "pos = " << (int)pos.x << "x" << (int)pos.y << std::endl << std::endl;
    }
    return pos;
}


unsigned int ThinkingAI::getPointsForCombination(const Meeple *meeple[4], const Meeple& meepleToSet) const{
    //take a property of the meepleToSet
    //go through all meeples in the set. If a meeple has the same property, add some points.
    //return the points. The caller than has to assign this points to all positions, where no meeple is currently present

    //color:
        int m;
        int color = 0;
        int size = 0;
        int shape = 0;
        int detail = 0;

        for (m = 0; m < 4; m++){
            if (meeple[m] == nullptr){
                continue;
            }
            if (meeple[m]->getColor() == meepleToSet.getColor() && color >= 0){
                color += 5;
            }else{  //color will never match -> no points
                color = -1;
            }
            if (meeple[m]->getSize() == meepleToSet.getSize() && size >= 0){
                size += 5;
            }else{  //color will never match -> no points
                size = -1;
            }
            if (meeple[m]->getShape() == meepleToSet.getShape() && shape >= 0){
                shape += 5;
            }else{  //color will never match -> no points
                shape = -1;
            }
            if (meeple[m]->getDetail() == meepleToSet.getDetail() && detail >= 0){
                detail += 5;
            }else{  //color will never match -> no points
                detail = -1;
            }
        }

        int bonus = 0;

        if (!pretendStupidity){
            if (color >= 15 || size >= 15 || shape >= 15 || detail >= 15){      //It's possible to win the game with this move
                bonus = 20;
            }
        }
        

        //Return the sum of all points:
        return bonus + (color > 0 ? color : 0) + (size > 0 ? size : 0) + (shape > 0 ? shape : 0) + (detail > 0 ? detail : 0);
}