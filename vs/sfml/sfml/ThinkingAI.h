#pragma once
#include "I_AI.h"

int getMaximum(int* intArray, unsigned int length);         //Returns the max. value within an int-array
float getAverage(int* intArray, unsigned int length);       //Returns the averaage value within an int-array

class ThinkingAI : public I_AI {
private:
    const bool intelligentMeepleChoosing;
    const bool intelligentMeeplePositioning;


    int* buildThinkingMap(const BoardState& board, const Meeple& meepleToSet) const;       //Note: the return-value has to be deleted[]
    int getPointsForCombination(const Meeple *meeple[4], const Meeple& meepleToSet) const;
    BoardPos getOptimalScoreMapPosition(int* scoreMap, bool printThinkMap);                //Searches for the field with the best score in the scoreMap, and returns its position
public:
    /*
        pretendStupidity:   false: when the AI is choosing a field to set the meeple, fields which can win the game are always chosen
                            true: no "if I can win it with this move, do it" is used; the AI always sets on fields which are strategically usefull for the future
    */
    ThinkingAI(bool intelligentMeepleChoosing, bool intelligentMeeplePositioning);
    virtual const Meeple& selectOpponentsMeeple(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board);
    virtual BoardPos selectMeeplePosition(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board, const Meeple& meepleToSet);
};

