#pragma once
#include "I_AI.h"
class ThinkingAI : public I_AI {
private:
    const bool pretendStupidity;


    unsigned int getPointsForCombination(const Meeple *meeple[4], const Meeple& meepleToSet) const;
public:
    /*
        pretendStupidity:   false: when the AI is choosing a field to set the meeple, fields which can win the game are always chosen
                            true: no "if I can win it with this move, do it" is used; the AI always sets on fields which are strategically usefull for the future
    */
    ThinkingAI(bool pretendStupidity); 
    virtual const Meeple& selectOpponentsMeeple(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board);
    virtual BoardPos selectMeeplePosition(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board, const Meeple& meepleToSet);
};

