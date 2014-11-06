#pragma once

#include <vector>
#include <set>

#include "helper.h"
#include "meeple.h"



//All meeples a player has (8 different meeples)
class MeepleBag
{
private:
    std::set<Meeple*> usedMeeples;
    std::set<Meeple*> meeples;
public:
    explicit MeepleBag(MeepleColor::Enum color);            //creates a new bag with 8 brand new meeples
    virtual ~MeepleBag();
    void reset();                                           //Reinitialises the object

    unsigned int getMeepleCount() const;                    //returns the number of meeples in the bag

    unsigned int getSimilarMeepleCount(MeepleColor::Enum color) const;      //Returns the number of meeples with the same color
    unsigned int getSimilarMeepleCount(MeepleSize::Enum size) const;        //Returns the number of meeples with the same size
    unsigned int getSimilarMeepleCount(MeepleShape::Enum shape) const;      //Returns the number of meeples with the same shape
    unsigned int getSimilarMeepleCount(MeepleDetail::Enum detail) const;    //Returns the number of meeples with the same detail
    
    const std::set<const Meeple*>* getMeeples() const;      //returns all meeples in the bag
        
    Meeple* removeMeeple(const Meeple& meeple);             //removes the meeple from the bag. NOTE: the meeple will not be deleted
    bool isMeepleInBag(const Meeple& meeple) const;         //returns, if this meeple is in the bag
};

