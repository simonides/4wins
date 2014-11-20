#pragma once

#include "ThinkingAI.h"


class SmartAI : public ThinkingAI {
    private:
        int getPointsForCombination_combineMeeples(const GameState& gameState, const WinCombination& winCombination, const Meeple& meepleToSet) const;
        float getPointsForCombination_blockOpponent(const GameState& gameState, const WinCombination& winCombination, const Meeple& meepleToSet) const;
    protected:
        virtual int getPointsForCombination(const GameState& gameState, const WinCombination& winCombination, const Meeple& meepleToSet) const;
    public:
        SmartAI(bool intelligentMeepleChoosing = true, bool intelligentMeeplePositioning = true);
};