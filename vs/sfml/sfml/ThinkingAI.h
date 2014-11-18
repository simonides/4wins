#pragma once
#include "I_AI.h"
struct WinCombination;

//The following functions are used to select the opponent's meeple:
    int getMaximum(int* intArray, unsigned int length);         //Returns the max. value within an int-array
    float getAverage(int* intArray, unsigned int length);       //Returns the averaage value within an int-array
// ~ ~ ~ ~ ~ ~
class ThinkingAI : public I_AI{
private:
    const bool intelligentMeepleChoosing;
    const bool intelligentMeeplePositioning;

    int* buildScoreMap(const GameState& gameState, const Meeple& meepleToSet) const;        //Caclulated the points for each combination, and sums the points up for each field on the board; the field with the highest points should get chosen for meeple positioning; Note: the return-value has to be deleted[]
    BoardPos getOptimalScoreMapPosition(int* scoreMap, bool printScoreMap);                 //Searches for the field with the best score in the scoreMap, and returns its position
 
    ThinkingAI& operator = (const ThinkingAI&);
protected:
    virtual int getPointsForCombination(const GameState& gameState, const WinCombination& winCombination, const Meeple& meepleToSet) const;

public:
    ThinkingAI(bool intelligentMeepleChoosing = true, bool intelligentMeeplePositioning = true);

    virtual const Meeple& selectOpponentsMeeple(const GameState& gameState);
    virtual BoardPos selectMeeplePosition(const GameState& gameState, const Meeple& meepleToSet);
};
