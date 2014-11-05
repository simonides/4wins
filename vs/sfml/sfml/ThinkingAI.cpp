#include "ThinkingAI.h"

#include "BoardState.h"
#include "MeepleBag.h"
#include "config.h"

#include <iostream>
#include <assert.h>


int getMaximum(int* intArray, unsigned int length){
    assert(length > 0);
    int max = intArray[0];
    for (unsigned int i = 1; i < length; i++){
        if (intArray[i] > max){
            max = intArray[i];
        }
    }
    return max;
}

float getAverage(int* intArray, unsigned int length){
    assert(length > 0);
    long sum = 0;
    for (unsigned int i = 0; i < length; i++){
        sum += intArray[i];
    }
    return sum / (float)length;
}






ThinkingAI::ThinkingAI(bool intelligentMeepleChoosing, bool intelligentMeeplePositioning) : intelligentMeepleChoosing(intelligentMeepleChoosing), intelligentMeeplePositioning(intelligentMeeplePositioning){
}


struct MeeplePoints{ //Internal structure, which is needed during the computation of the meeple to be chosen
    const Meeple* meeple;
    int points;
};


const Meeple& ThinkingAI::selectOpponentsMeeple(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board){
    if (!intelligentMeepleChoosing){
        return **(opponentBag.getMeeples()->cbegin());
    }
        
    /* We take one meeple after another from the opponent's bag
    We calculate the scoreMap, which the opponent would get, if we would choose the meeple.
    --> calculate the best score, and the average score for the map
    --> calculate the points out of these 2 values --> these are the points for the meeple
    --> store the meeple and its points in a MeeplePoints-set
    Afterwards, select the meeple with the lowest points
    */
    
    const WinCombinationSet* allCombinations = board.getWinCombinations();
    const std::set<const Meeple*>* meeples = opponentBag.getMeeples();      //we have to choose one of these meeples

    MeeplePoints* points = new MeeplePoints[meeples->size()];               //Each meeple in the opponents bag will get an entry .. soon
    

    if (PRINT_THINK_MAP){
        std::cout << "Thinkingmap for selecting a meeple:" << std::endl;
    }

    int* scoreMap;
    int f=0;
    for (std::set<const Meeple*>::const_iterator it = meeples->begin(); it != meeples->end(); ++it, ++f){
        scoreMap = buildThinkingMap(board, **it);     //The opponent gets this map, if this meeple is chosen
        
        int best = getMaximum(scoreMap, 4 * 4);
        float avg = getAverage(scoreMap, 4 * 4);
        
        delete[] scoreMap;

        points[f].meeple = *it;
        points[f].points = best * 8 + (int)(avg * 2.f);  //The higher this score, the better for the opponent

        if (PRINT_THINK_MAP){
            std::cout << (*it)->toString() << ":  Score = " << points[f].points << " (max: " << best << ", avg: " << avg << ")" << std::endl;
        }
    }

    //Select the lowest score:
    MeeplePoints* lowest = &points[--f];
    while (--f >= 0){
        if (points[f].points < lowest->points){
            lowest = &points[f];
        }
    }

    const Meeple* selection = lowest->meeple;
    delete[] points;

    if (PRINT_THINK_MAP){
        std::cout << "Choosing meeple " << selection->toString() << std::endl << std::endl;
    }
    return *selection;
}


int* ThinkingAI::buildThinkingMap(const BoardState& board, const Meeple& meepleToSet) const{
    int* scoreMap = new int[4*4];
    for (int i = 0; i < 4 * 4; i++){
        scoreMap[i] = 0;
    }

    //go through the whole map, and add points if the meeple should be set there
    //  --> check all possible combinations, which can lead to a victory
    //  --> look at all meeples, that are already placed in the combination
    //  --> check, if these meeples have any similarities with the meepleToSet. Calculate matching-points - the more similarities, the better
    //  --> add the matching-points to all fields in the combination, where no meeple is currently placed
    //(Afterwards, go through the scoreMap, and choose the field with the most points)

    const WinCombinationSet* allCombinations = board.getWinCombinations();
    for (std::set<WinCombination*>::const_iterator it = allCombinations->combination.begin(); it != allCombinations->combination.end(); it++){
        WinCombination* comb = *it;
        int points = getPointsForCombination(comb->meeples, meepleToSet);
        for (int m = 0; m < 4; m++){
            if (comb->meeples[m] == nullptr){    //The field is empty --> add the points
                scoreMap[comb->positions[m].x + 4*comb->positions[m].y] += points +1;           //+1 --> occupied fields have 0, free fields >0. Diagonal fields have automatically 1 point more than others
            }
        }
    }
    return scoreMap;
}



//Finds the highest value in the scoreMap, and returns the position of the field. Optionally prints the scoreMap
BoardPos ThinkingAI::getOptimalScoreMapPosition(int* scoreMap, bool printThinkMap){
    int points = scoreMap[0];
    uint8_t pos = 0;
    unsigned int randMod = 2;

    if (printThinkMap){
        std::cout << "Thinkingmap for choosing a position:" << std::endl;
    }
    for (uint8_t f = 0; f < 4 * 4; f++){
        if (printThinkMap){
            std::cout << scoreMap[f];
        }

        if (scoreMap[f] > points || (scoreMap[f] == points && (rand() % (randMod++)) == 0)){
            points = scoreMap[f];
            pos = f;
            if (printThinkMap){
                std::cout << "! ";
            }
        }else if (printThinkMap){
            std::cout << "  ";
        }

        if (printThinkMap && (f+1) % 4 == 0){
            std::cout << std::endl;
        }
    }
    
    BoardPos position = { pos % 4, pos / 4 };

    if (printThinkMap){
        std::cout << "pos = " << (int)position.x << "x" << (int)position.y << std::endl << std::endl;
    }
    return position;
}



BoardPos ThinkingAI::selectMeeplePosition(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board, const Meeple& meepleToSet){
    if (!intelligentMeeplePositioning){
        BoardPos pos;
        do{
            pos.x = rand() % 4;
            pos.y = rand() % 4;
        } while (!board.isFieldEmpty(pos));
        return pos;
    }
        
    int* scoreMap = buildThinkingMap(board, meepleToSet);
    BoardPos pos = getOptimalScoreMapPosition(scoreMap, PRINT_THINK_MAP);
   
    delete[] scoreMap;
    return pos;
}


int ThinkingAI::getPointsForCombination(const Meeple *meeple[4], const Meeple& meepleToSet) const{
    //take each property of the meepleToSet
    //go through all meeples in the combination-set. If a meeple has the same property, add some points.
    //if the meeple has a different property, remove some points (we would block the field for further usage)

    //return the points. The caller than has to assign this points to all positions, where no meeple is currently present
    
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
        }
        else{  //color will never match -> negative points (we would destroy the opportunity to use this field later)
            color = -2;
        }
        if (meeple[m]->getSize() == meepleToSet.getSize() && size >= 0){
            size += 5;
        }
        else{
            size = -2;
        }
        if (meeple[m]->getShape() == meepleToSet.getShape() && shape >= 0){
            shape += 5;
        }
        else{
            shape = -2;
        }
        if (meeple[m]->getDetail() == meepleToSet.getDetail() && detail >= 0){
            detail += 5;
        }
        else{
            detail = -2;
        }
    }

    //Note: the AI can be improved here. Go through all properties with -2, and check if we are really blocking possible future-movements (which can be an advantage for the AI)
    //if not, set it to 0


    int bonus = 0;
    if (true){
        if (color >= 15 || size >= 15 || shape >= 15 || detail >= 15){      //It's possible to win the game with this move
            bonus = 20;
        }
    }

    //Return the sum of all points:
    return bonus + color + size + shape + detail;
}
