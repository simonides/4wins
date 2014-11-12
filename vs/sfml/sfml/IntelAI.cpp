#include "IntelAI.h"

#include "Board.h"
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









#define HIGHEST_SINGLE_POINTS 1000


int IntelAI::getPointsForCombination(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Meeple& meepleToSet, const Meeple *meeple[4]) const{
    
    int points = getPointsForCombination_combineMeeples(ownBag, opponentBag, meepleToSet, meeple);
    assert(points <= HIGHEST_SINGLE_POINTS);
    
    if (points >= HIGHEST_SINGLE_POINTS){       //we can win the game --> return lots of points, so that we will choose that fields
        return points;
    }

    return points - getPointsForCombination_blockOpponent(ownBag, opponentBag, meepleToSet, meeple);       
}





int IntelAI::getPointsForCombination_combineMeeples(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Meeple& meepleToSet, const Meeple *meeple[4]) const{
    //Returns points, if we can form similarities by placing a meeple
    //take each property of the meepleToSet
    //count the number of meeples in the combination, which have the same property
    //  if this number is 3, return a huge number (we win, if we set the meeple there)
    //  if the numer is 2: check, if the opponent has meeples which could win the combination. return a low number, depending on the amount of meeples the opponent has
    //  if 1: return a small number

    int m;
    //The following values store the number of meeples, who share the same property as the meepleToSet
    uint8_t match[4] = { 0 };    
    uint8_t empty = 0;

    for (m = 0; m < 4; ++m){
        if (meeple[m] == nullptr){
            empty++;
            continue;
        }
        for (int p = 0; p < 4; ++p){    //for each property
            if (meeple[m]->getProperty(p) == meepleToSet.getProperty(p)){
                match[p]++;
            }
        }
    }

    if (empty == 4 || empty == 0){  //Nothing todo
        return 0;
    }

    int points = 0;
    for (m = 0; m < 4; ++m){
        if (match[m] >= 3){                         //if we set the meeple there, we can win the game
            return HIGHEST_SINGLE_POINTS;           //always choose this position
        }

        if (match[m] == 2){         //we MUST NOT set the meeple there, if the opponent has a meeple that could win the game
            //Set the points, depending on how many of the opponent's meeples don't match
            //If all the opponent's meeples match this property --> never set here!                

            unsigned int oppnentMatches = opponentBag.getSimilarMeepleCount( meepleToSet.getProperty(m) );
            oppnentMatches /= opponentBag.getMeepleCount();     //percent-value

            points -= static_cast<int>(oppnentMatches / static_cast<float>(opponentBag.getMeepleCount()) * 50.f); //worst case: happens at 11 properties (4*2 + 3). The 12 property could lead to a win. --> the value must not exceed HIGHEST_SINGLE_POINTS/11 (=90)

        }
        if (match[m] == 1){         //best case: happens at 11 properties (4*2 + 3). The 12. property could be a "never set there" --> the value therefore must be < 50/11 (=4)
            points++;
        }
    }
    return points;
}




int IntelAI::getPointsForCombination_blockOpponent(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Meeple& meepleToSet, const Meeple *meeple[4]) const{
    //Returns points, if we can avoid that our opponent creates similarities
    
    //check, if the meeples on the board share any property (every meeple has the same property)
    //If yes: check, if the opponent has meeples, which also have the same property
    //  If also yes: return points, depending on how many meeples share the same property, and on how many meeples the opponent has with this property

    return 0;
    //todo: hier weiter programmieren






  //  int m;
   /* int color = -1;
    int colorMatches = 0;
    /*int size = 0;
    int shape = 0;
    int detail = 0;* /


    
    for (m = 0; m < 4; ++m){
        if (meeple[m] == nullptr){
            continue;
        }
        if (color == -1){ 
            color = meeple[m]->getColor(); 
            colorMatches++;
        }else if (meeple[m]->getColor() == color && colorMatches>=0){
            colorMatches++;
        }else{
            colorMatches = -1;
        }







       
    }*/












    

    /*

    //The following values store the number of meeples, who share the same property as the meepleToSet
    uint8_t match[4] = { 0 };
    uint8_t& color = match[0];
    uint8_t& size = match[1];
    uint8_t& shape = match[2];
    uint8_t& detail = match[3];
    uint8_t empty = 0;

    for (m = 0; m < 4; ++m){
        if (meeple[m] == nullptr){
            empty++;
            continue;
        }
        color += (meeple[m]->getColor() == meepleToSet.getColor()) ? 1 : 0;
        size += (meeple[m]->getSize() == meepleToSet.getSize()) ? 1 : 0;
        shape += (meeple[m]->getShape() == meepleToSet.getShape()) ? 1 : 0;
        detail += (meeple[m]->getDetail() == meepleToSet.getDetail()) ? 1 : 0;
    }

    if (empty == 4 || empty == 0){  //Nothing todo
        return 0;
    }

    int points = 0;
    for (m = 0; m < 4; ++m){
        if (match[m] >= 3){                         //if we set the meeple there, we can win the game
            return HIGHEST_SINGLE_POINTS;            //always choose this position
        }

        if (match[m] == 2){         //we MUST NOT set the meeple there, if the opponent has a meeple that could win the game
            //Set the points, depending on how many of the opponent's meeples don't match
            //If all the opponent's meeples match this property --> never set here!                

            unsigned int oppnentMatches;
            switch (m){
            case 0: oppnentMatches = opponentBag.getSimilarMeepleCount(meepleToSet.getColor());  break;
            case 1: oppnentMatches = opponentBag.getSimilarMeepleCount(meepleToSet.getSize());   break;
            case 2: oppnentMatches = opponentBag.getSimilarMeepleCount(meepleToSet.getShape());  break;
            case 3: oppnentMatches = opponentBag.getSimilarMeepleCount(meepleToSet.getDetail()); break;
            }
            oppnentMatches /= opponentBag.getMeepleCount();     //percent-value

            points -= static_cast<int>(oppnentMatches / static_cast<float>(opponentBag.getMeepleCount()) * 50.f); //worst case: happens at 11 properties (4*2 + 3). The 12 property could lead to a win. --> the value must not exceed HIGHEST_SINGLE_POINTS/11 (=90)

        }
        if (match[m] == 1){         //best case: happens at 11 properties (4*2 + 3). The 12. property could be a "never set there" --> the value therefore must be < 50/11 (=4)
            points++;
        }
    }
    return points;





    */









    /*



    unsigned int meepleCount = opponentBag.getMeepleCount();
    
    unsigned int canWin = 0;
    for (unsigned int m = 0; m < meepleCount; ++m){ //for each of the opponent's meeples
        int points = getPointsForCombination_combineMeeples(opponentBag, ownBag, *opponentBag.getMeeple(m), meeple);    //pretend that we are the opponent
        if (points >= HIGHEST_SINGLE_POINTS){    //The opponent wins, if we don't set a meeple to block him
            canWin++;
        }
    }
    
    return static_cast<int>(canWin / static_cast<float>(opponentBag.getMeepleCount()) * 80.f); //worst case: happens at 11 properties (4*2 + 3). The 12 property could lead to a win. --> the value must not exceed HIGHEST_SINGLE_POINTS/11 (=90)   
    */
}
