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


IntelAI::IntelAI(bool intelligentMeepleChoosing, bool intelligentMeeplePositioning) : ThinkingAI(intelligentMeepleChoosing, intelligentMeeplePositioning){
}


int IntelAI::getPointsForCombination(const Meeple *meeple[4], const Meeple& meepleToSet, const MeepleBag& ownBag, const MeepleBag& opponentBag) const{
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
