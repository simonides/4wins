#pragma once
#include "I_AI.h"

int getMaximum(int* intArray, unsigned int length);         //Returns the max. value within an int-array
float getAverage(int* intArray, unsigned int length);       //Returns the averaage value within an int-array

class ThinkingAI : public I_AI {
private:
    const bool intelligentMeepleChoosing;
    const bool intelligentMeeplePositioning;


    int* buildThinkingMap(const BoardState& board, const MeepleBag& ownBag, const MeepleBag& opponentBag, const Meeple& meepleToSet) const;       //Note: the return-value has to be deleted[]
    BoardPos getOptimalScoreMapPosition(int* scoreMap, bool printThinkMap);                 //Searches for the field with the best score in the scoreMap, and returns its position
protected:
    virtual int getPointsForCombination(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Meeple& meepleToSet, const Meeple *meeple[4]) const;

public:
    ThinkingAI(bool intelligentMeepleChoosing = true, bool intelligentMeeplePositioning = true);

    virtual const Meeple& selectOpponentsMeeple(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board);
    virtual BoardPos selectMeeplePosition(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board, const Meeple& meepleToSet);
};

