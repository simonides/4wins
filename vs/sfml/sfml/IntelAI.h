#pragma once

#include "ThinkingAI.h"


class IntelAI : public ThinkingAI {
protected:
    virtual int getPointsForCombination(const Meeple *meeple[4], const Meeple& meepleToSet, const MeepleBag& ownBag, const MeepleBag& opponentBag) const;
public:
    IntelAI(bool intelligentMeepleChoosing = true, bool intelligentMeeplePositioning = true);
};