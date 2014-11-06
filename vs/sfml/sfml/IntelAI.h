#pragma once

#include "ThinkingAI.h"


class IntelAI : public ThinkingAI {
    private:
        int getPointsForCombination_combineMeeples(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Meeple& meepleToSet, const Meeple *meeple[4]) const;
        int getPointsForCombination_blockOpponent(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Meeple& meepleToSet, const Meeple *meeple[4]) const;
    protected:
        virtual int getPointsForCombination(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Meeple& meepleToSet, const Meeple *meeple[4]) const;
    public:
        IntelAI(bool intelligentMeepleChoosing = true, bool intelligentMeeplePositioning = true);
};