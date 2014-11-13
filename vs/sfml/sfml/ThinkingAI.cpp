#include "ThinkingAI.h"

#include <iostream>
#include <assert.h>

#include "Game.h"
#include "Board.h"
#include "MeepleBag.h"
#include "config.h"

#pragma warning( disable: 4100 )


int getMaximum(int* intArray, unsigned int length){
    assert(length > 0);
    int max = intArray[0];
    for (unsigned int i = 1; i < length; ++i){
        if (intArray[i] > max){
            max = intArray[i];
        }
    }
    return max;
}

float getAverage(int* intArray, unsigned int length){
    assert(length > 0);
    long sum = 0;
    for (unsigned int i = 0; i < length; ++i){
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


const Meeple& ThinkingAI::selectOpponentsMeeple(const GameState& gameState) {
    if (!intelligentMeepleChoosing){
        return *gameState.opponentBag.getMeeple(0);
    }
        
    /* We take one meeple after another from the opponent's bag
    We calculate the scoreMap, which the opponent would get, if we would choose the meeple.
    --> calculate the best score, and the average score for the map
    --> calculate the points out of these 2 values --> these are the points for the meeple
    --> store the meeple and its points in a MeeplePoints-set
    Afterwards, select the meeple with the lowest points
    */
  
    unsigned int meepleCount = gameState.opponentBag.getMeepleCount();

    MeeplePoints* points = new MeeplePoints[meepleCount];                       //Each meeple in the opponents bag will get an entry .. soon
    

    #if PRINT_THINK_MAP
        std::cout << "Thinkingmap for selecting a meeple:" << std::endl;
    #endif

    int* scoreMap;
    int f = 0;
    for (; f < static_cast<int>(meepleCount); ++f){
        points[f].meeple = gameState.opponentBag.getMeeple(f);
        
        scoreMap = buildScoreMap(gameState, *points[f].meeple);          //The opponent gets this map, if this meeple is chosen
        
        int best = getMaximum(scoreMap, 4 * 4);
        float avg = getAverage(scoreMap, 4 * 4);
        
        delete[] scoreMap;

        points[f].points = best * 8 + (int)(avg * 2.f);  //The higher this score, the better for the opponent

        #if PRINT_THINK_MAP
            std::cout << points[f].meeple->toString() << ":  Score = " << points[f].points << " (max: " << best << ", avg: " << avg << ")" << std::endl;
        #endif
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

    #if PRINT_THINK_MAP
        std::cout << "Choosing meeple " << selection->toString() << std::endl << std::endl;
    #endif
    return *selection;
}

BoardPos ThinkingAI::selectMeeplePosition(const GameState& gameState, const Meeple& meepleToSet){
    if (!intelligentMeeplePositioning){
        return gameState.board.getRandomEmptyField();
    }

    int* scoreMap = buildScoreMap(gameState, meepleToSet);
    BoardPos pos = getOptimalScoreMapPosition(scoreMap, PRINT_THINK_MAP);

    delete[] scoreMap;
    return pos;
}






int* ThinkingAI::buildScoreMap(const GameState& gameState, const Meeple& meepleToSet) const{
    int* scoreMap = new int[4 * 4];//   todo: does initialising also work with this, instead of the loop?: {0};
    for (int i = 0; i < 4 * 4; ++i){
        scoreMap[i] = 0;
    }

//HIER{ 0 }


    //go through the whole map, and add points if the meeple should be set there
    //  --> check all possible combinations, which can lead to a victory (4 in a row/col/diagonal)
    //  --> look at all meeples, that are already placed in the combination (0-4 meeples)
    //  --> check, if these meeples have any similarities with the meepleToSet. Calculate matching-points - the more similarities, the better
    //  --> add the matching-points to all fields in the combination, where no meeple is currently placed
    //(Afterwards, go through the scoreMap, and choose the field with the most points)

    const WinCombinationSet* allCombinations = gameState.board.getWinCombinations();
    for (std::set<WinCombination*>::const_iterator it = allCombinations->combination.begin(); it != allCombinations->combination.end(); ++it){
        WinCombination* comb = *it;
        int points = getPointsForCombination(gameState, *comb, meepleToSet);
        for (int m = 0; m < 4; ++m){
            uint8_t field = comb->positions[m].x + 4 * comb->positions[m].y;        //convert 2D-coords [x][y] to [0-15] value (0 = top left, 3 = top right)
            if (comb->meeples[m] == nullptr){       //The field is empty, the meeple can be set here --> add the points
                scoreMap[field] += points + 1;      //+1 --> diagonal fields have automatically 1 point more than others (the more combinations a field is part of, the more points it gets ---> AIs prefer diagonals)
                assert(scoreMap[field] > -10000);   //everything else would make no sense
            }else{
                scoreMap[field] = INT_MIN;
            }
        }
    }
    return scoreMap;
}



//Finds the highest value in the scoreMap, and returns the position of the field. Optionally prints the scoreMap
//If there are several fields with the same score, this function chooses a field randomly
BoardPos ThinkingAI::getOptimalScoreMapPosition(int* scoreMap, bool printScoreMap){
    int points = scoreMap[0];
    uint8_t pos = 0;
    unsigned int randMod = 2;   //needed for random choosing between fields with the same score (to provide the same propability for all fields)

    if (printScoreMap){
        std::cout << "Thinkingmap for choosing a position:" << std::endl;
    }
    for (uint8_t f = 0; f < 4 * 4; ++f){
        if (printScoreMap){
            std::cout << scoreMap[f];
        }

        if (scoreMap[f] > points || (scoreMap[f] == points && (rand() % (randMod++)) == 0)){
            points = scoreMap[f];
            pos = f;
            if (scoreMap[f] > points){
                randMod = 2;
            }
            if (printScoreMap){
                std::cout << "! ";              //symbol for "this field has been selected"
            }
        }else if (printScoreMap){
            std::cout << "  ";
        }

        if (printScoreMap && (f + 1) % 4 == 0){
            std::cout << std::endl;
        }
    }
    
    BoardPos position = { pos % 4, pos / 4 };   //convert value [0,15] into 2D coordinates [0-3][0-3]

    if (printScoreMap){
        std::cout << "pos = " << (int)position.x << "x" << (int)position.y << std::endl << std::endl;
    }
    return position;
}





int ThinkingAI::getPointsForCombination(const GameState& gameState, const WinCombination& winCombination, const Meeple& meepleToSet) const{
    //returns points for all un-occupied fields in the winCombination (more points, if the AI should place it's meeple there)
    // --> take each property of the meepleToSet
    // --> go through all meeples in the combination-set. If a meeple has the same property, add some points.
    // --> if the meeple has a different property, remove some points (we would block the field for further usage)

    //return the points. The caller than has to assign this points to all positions, where no meeple is currently present
    
    int m, p;
    int propPoints[4] = { 0 };  //Each property (color, shape, size, detail) gets its own similarity-points

    for (m = 0; m < 4; ++m){
        if (winCombination.meeples[m] == nullptr){
            continue;
        }
        for (p = 0; p < 4; ++p){    //For each property
            if (propPoints[p] < 0){ //we already know that his property can never lead to a win
                break;
            }
            if (winCombination.meeples[m]->getProperty(p) == meepleToSet.getProperty(p)){
                propPoints[p] += 5;
            }else{  //property will never match -> negative points (we would destroy the opportunity to use this field later for another combination)
                propPoints[p] = -2;
                break;
            }
        }
    }

   
    int bonus = 0;
    int sum = 0;
    for (p = 0; p < 4; p++){
        sum += propPoints[p];
        if (propPoints[p] >= 15){      //It's possible to win the game with this move
            bonus = 20;
        }
    }

    return bonus +sum;
}
