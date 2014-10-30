#pragma once

#include <vector>
#include <set>

#include "helper.h"
#include "meeple.h"



//All meeples a player has (8 different meeples)
class MeepleBag
{
private:
    std::set<Meeple*> meeples;

public:
    MeepleBag(MeepleColor color);           //creates a new bag with 8 brand new meeples
    virtual ~MeepleBag();

    std::set<Meeple*>::const_iterator getMeeples() const;                //returns all meeples in the bag
    void removeMeeple(Meeple& meeple);      //removes the meeple from the bag
    bool isMeepleInBag(Meeple& meeple) const;     //returns, if this meeple is in the bag
};

