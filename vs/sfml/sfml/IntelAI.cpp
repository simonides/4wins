#include "IntelAI.h"

#include "BoardState.h"
#include "MeepleBag.h"
#include "config.h"

#include <iostream>
#include <assert.h>




/* Implemented in ThinkingAI: */
    int getMaximum(int* intArray, unsigned int length);         //Returns the max. value within an int-array
    float getAverage(int* intArray, unsigned int length);       //Returns the averaage value within an int-array
/* ~ ~ ~ ~ ~ ~*/






IntelAI::IntelAI(bool intelligentMeepleChoosing, bool intelligentMeeplePositioning) : intelligentMeepleChoosing(intelligentMeepleChoosing), intelligentMeeplePositioning(intelligentMeeplePositioning){
}

struct MeeplePoints{ //Internal structure, which is needed during the computation of the meeple to be chosen
    const Meeple* meeple;
    int points;
};

const Meeple& IntelAI::selectOpponentsMeeple(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board){
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
    int f = 0;
    for (std::set<const Meeple*>::const_iterator it = meeples->begin(); it != meeples->end(); ++it, ++f){
        scoreMap = buildThinkingMap(board, **it, opponentBag);     //The opponent gets this map, if this meeple is chosen

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

int* IntelAI::buildThinkingMap(const BoardState& board, const Meeple& meepleToSet, const MeepleBag& opponentBag) const{
    int* scoreMap = new int[4 * 4];
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
        int points = getPointsForCombination(comb->meeples, meepleToSet, opponentBag);
        for (int m = 0; m < 4; m++){
            uint8_t field = comb->positions[m].x + 4 * comb->positions[m].y;
            if (comb->meeples[m] == nullptr){    //The field is empty --> add the points
                scoreMap[field] += points + 1;           //+1 --> occupied fields have 0, free fields >0. Diagonal fields have automatically 1 point more than others
                assert(scoreMap[field] > -100);
            }
            else{
                scoreMap[field] = INT_MIN;
            }
        }
    }
    return scoreMap;
}

//Finds the highest value in the scoreMap, and returns the position of the field. Optionally prints the scoreMap
BoardPos IntelAI::getOptimalScoreMapPosition(int* scoreMap, bool printThinkMap){
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
        }
        else if (printThinkMap){
            std::cout << "  ";
        }

        if (printThinkMap && (f + 1) % 4 == 0){
            std::cout << std::endl;
        }
    }

    BoardPos position = { pos % 4, pos / 4 };

    if (printThinkMap){
        std::cout << "pos = " << (int)position.x << "x" << (int)position.y << std::endl << std::endl;
    }
    return position;
}

BoardPos IntelAI::selectMeeplePosition(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board, const Meeple& meepleToSet){
    if (!intelligentMeeplePositioning){
        BoardPos pos;
        do{
            pos.x = rand() % 4;
            pos.y = rand() % 4;
        } while (!board.isFieldEmpty(pos));
        return pos;
    }

    int* scoreMap = buildThinkingMap(board, meepleToSet, opponentBag);
    BoardPos pos = getOptimalScoreMapPosition(scoreMap, PRINT_THINK_MAP);

    delete[] scoreMap;
    return pos;
}


int IntelAI::getPointsForCombination(const Meeple *meeple[4], const Meeple& meepleToSet, const MeepleBag& opponentBag) const{
    //take each property of the meepleToSet
    //count the number of meeples in the combination, which have the same property
    //  if this number is 3, return a huge number (we win, if we set the meeple there)
    //  if the numer is 2: check, if the opponent has meeples which could win the combination. return a low number, depending on the amount of meeples the opponent has
    //  if 1: return a small number

    int m;
    //The following values store the number of meeples, who share the same property as the meepleToSet
    uint8_t match[4] = { 0 };
        uint8_t& color  = match[0];
        uint8_t& size   = match[1];
        uint8_t& shape  = match[2];
        uint8_t& detail = match[3];

    for (m = 0; m < 4; m++){
        if (meeple[m] == nullptr){
            continue;
        }
        color  += (meeple[m]->getColor()  == meepleToSet.getColor() ) ? 1 : 0;
        size   += (meeple[m]->getSize()   == meepleToSet.getSize()  ) ? 1 : 0;
        shape  += (meeple[m]->getShape()  == meepleToSet.getShape() ) ? 1 : 0;
        detail += (meeple[m]->getDetail() == meepleToSet.getDetail()) ? 1 : 0;
    }

    int points = 0;
    for (m = 0; m < 4; m++){
        if (match[m] >= 3){         //if we set the meeple there, we can win the game
            return 1000;            //always choose this position
        }

        if (match[m] == 2){         //we MUST NOT set the meeple there, if the opponent has a meeple that could win the game
            //Set the points, depending on how many of the opponent's meeples don't match
            //If all the opponent's meeples match this property --> never set here!                
            
            float oppnentMatches;
            switch (m){
                case 0: oppnentMatches = static_cast<float>(opponentBag.getSimilarMeepleCount(meepleToSet.getColor() )); break;
                case 1: oppnentMatches = static_cast<float>(opponentBag.getSimilarMeepleCount(meepleToSet.getSize()  )); break;
                case 2: oppnentMatches = static_cast<float>(opponentBag.getSimilarMeepleCount(meepleToSet.getShape() )); break;
                case 3: oppnentMatches = static_cast<float>(opponentBag.getSimilarMeepleCount(meepleToSet.getDetail())); break;
            }
            oppnentMatches /= opponentBag.getMeepleCount();     //percent-value
            
            points -= (int)(oppnentMatches * 50); //worst case: happens at 11 properties (4*2 + 3). The 12 property could lead to a win. --> the value must not exceed 1000/11
        
        }
        if (match[m] == 1){         //best case: happens at 11 properties (4*2 + 3). The 12. property could be a "never set there" --> the value therefore must never be >=50/11
            points++;
        }
    }

    return points;
}
