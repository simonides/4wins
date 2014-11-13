#include "SmartAI.h"

#include "Game.h"
#include "Board.h"
#include "MeepleBag.h"
#include "config.h"

#include <iostream>
#include <assert.h>





SmartAI::SmartAI(bool intelligentMeepleChoosing, bool intelligentMeeplePositioning) : ThinkingAI(intelligentMeepleChoosing, intelligentMeeplePositioning){
}









#define HIGHEST_SINGLE_POINTS 1000


int SmartAI::getPointsForCombination(const GameState& gameState, const WinCombination& winCombination, const Meeple& meepleToSet) const{
    int points = getPointsForCombination_combineMeeples(gameState, winCombination, meepleToSet);
    assert(points <= HIGHEST_SINGLE_POINTS);
    
    if (points >= HIGHEST_SINGLE_POINTS){       //we can win the game --> return lots of points, so that we will choose that field
        return points;
    }

    float reduction = getPointsForCombination_blockOpponent(gameState, winCombination, meepleToSet);
    if (points > 0){
        return static_cast<int>(points * (100.f - reduction) / 100.f);      //Remove 0-100% from the already calculated points
    }else{
        return static_cast<int>(points *reduction / 100.f);                    
    }
    
}





int SmartAI::getPointsForCombination_combineMeeples(const GameState& gameState, const WinCombination& winCombination, const Meeple& meepleToSet) const{
    //Returns points, if we can form similarities by placing a meeple
    //take each property of the meepleToSet
    //count the number of meeples in the combination, which have the same property
    //  if this number is 3, return a huge number (we win, if we set the meeple there)
    //  if the numer is 2: check, if the opponent has meeples which could win the combination. return a low number, depending on the amount of meeples the opponent has
    //  if 1: return a small number

    int m, p;
    //The following values store the number of meeples, who share the same property as the meepleToSet
    uint8_t match[4] = { 0 };
    uint8_t empty = 0;

    for (m = 0; m < 4; ++m){
        if (winCombination.meeples[m] == nullptr){
            empty++;
            continue;
        }
        for (p = 0; p < 4; ++p){    //for each property
            if (winCombination.meeples[m]->getProperty(p) == meepleToSet.getProperty(p)){
                match[p]++;
            }/*else{
                match[p] = 100;     //this property can never match -> mark it (we set it later to 0)
            }*/
        }
    }
    /*for (p = 0; p < 4; ++p){
        if (match[p] >= 100){
            match[p] = 0;
        }
    }*/
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
            unsigned int opponentMeepleCount = gameState.opponentBag->getMeepleCount();
            if (opponentMeepleCount == 0){
                ++points;
                continue;
            }
            unsigned int opponentMatches = gameState.opponentBag->getSimilarMeepleCount(meepleToSet.getProperty(m));
            opponentMatches /= opponentMeepleCount;     //percent-value

            points -= static_cast<int>(opponentMatches / static_cast<float>(opponentMeepleCount) * 50.f); //worst case: happens at 11 properties (4*2 + 3). The 12 property could lead to a win. --> the value must not exceed HIGHEST_SINGLE_POINTS/11 (=90)

        }
        if (match[m] == 1){         //best case: happens at 11 properties (4*2 + 3). The 12. property could be a "never set there" --> the value therefore must be < 50/11 (=4)
            ++points;
        }
    }
    return points;
}




float SmartAI::getPointsForCombination_blockOpponent(const GameState& gameState, const WinCombination& winCombination, const Meeple& meepleToSet) const{
    //Returns points, if we can avoid that our opponent creates combinations / combines similar meeples
    //  (the return-value is actually in percent: 100% = block it! now!   0% -->  can't block anything    
    // --> check, if the meeples on the board share any property (every meeple has the same property)
    // --> If the meeples share a property: 
    //          check if the meeple we have to set is able to block the property WITHOUT helping the opponent
    // --> if we can block the opponent: check, if the opponent has meeples, which also have the same property
    // --> If also yes: return points, depending on how many meeples share the same property, and on how many meeples the opponent has with this property
        
    
    int8_t match[4] = { 0 };           //stores the number of meeples in the combination, who share the same property
    const Meeple* ancestor = nullptr;   //The meeple, with which we compare the properties
    uint8_t empty = 0;                  //Number of empty fields in the combination

    int m, p;

    for (m = 0; m < 4; ++m){
        if (winCombination.meeples[m] == nullptr){
            empty++;
            continue;
        }
        if (ancestor == nullptr){
            ancestor = winCombination.meeples[m];

            for (p = 0; p < 4; p++){
                match[p] = 1;
            }
            continue;
        }
        for (p = 0; p < 4; ++p){    //for each property
            if (match[p] < 0){      //Can never match, one meeple has a different property
                continue;
            }
            if (winCombination.meeples[m]->getProperty(p) == ancestor->getProperty(p)){
                match[p]++;
            }else{
                match[p] = -1;      //no match
            }
        }
    }
    if (empty == 0 || empty == 4){      //nothing todo (we can't block anything, if there are no meeples/no empty fields to set)
        return 0;
    }
    uint8_t blockProp = 0;
    uint8_t helpProp = 0;
    for (p = 0; p < 4; p++){
        if (match[p] < 0){
            match[p] = 0;
        }
        if (ancestor->getProperty(p) == meepleToSet.getProperty(p)){        //we can't block the opponent with our meeple
            if (match[p] == 2){     //NEVER EVER set the meeple here - we would allow the enemy to win
                //todo: only aborthere, if the enemy has still meeples that could help him to win here
                return 0;
            }
            if (match[p] % 2 == 0){
                ++helpProp;
            }
            match[p] = 0;
        }else{
            if (match[p] % 2 == 1){
                ++blockProp;
            }
        }
    }
    if (helpProp >= blockProp){     //It makes no sense to block the player here, because we are helping him with much more properties
        return 0;
    }
    

    //Current situation:
    //  ancestor = contains the 4 Properties
    //  match[] ... contains the number of meeples, which share this property (0-3)
    //Now:
    // --> For each property, check how many meeples the opponent has, that match the same property
    // --> calculate the percentage of how many meeples..
    // --> calculate the importance: the more matches we had (max. 3), the higher the importance (3 meeples = 100%, 0 meeples = 0%)
    // --> find the highest importance

    unsigned int meepleCount = gameState.opponentBag->getMeepleCount();
    float highestImportance = 0.f;     //highest importance

    for (p = 0; p < 4; ++p){
        float similarMeeplesInBag = 100.f * gameState.opponentBag->getSimilarMeepleCount(ancestor->getProperty(p)) / static_cast<float>(meepleCount);   //how much percent of the opponents meeple's match this property? 

        float importance = similarMeeplesInBag * match[p] / 3;   
        //  Importance:
        //      0 matches --> 0%
        //      1 match   --> 33% * similarMeeplesInBag
        //      2 matches --> 66% * similarMeeplesInBag
        //      3 matches --> 100% * similarMeeplesInBag  

        if (importance > highestImportance){
            highestImportance = importance;
        }
    }
    
    return highestImportance;
}
