#pragma once

#include <vector>

#include "helper.h"
#include "meeple.h"




//All meeples a player has (8 different meeples)
class MeepleBag
{
private:
    std::vector<Meeple*> usedMeeples;
    std::vector<Meeple*> meeples;
public:
    explicit MeepleBag(MeepleColor::Enum color);            //creates a new bag with 8 brand new meeples
    virtual ~MeepleBag();
    void reset();                                           //Reinitialises the object

    unsigned int getMeepleCount() const;                    //returns the number of meeples in the bag

    unsigned int getSimilarMeepleCount(MeepleProperty prop) const;          //Returns the number of meeples with the same property value
    
    const Meeple* getMeeple(unsigned int index) const;      //Returns a specific meeple from the set
        
    Meeple* removeMeeple(const Meeple& meeple);             //removes the meeple from the bag
    Meeple* removeMeeple(unsigned int index);               //removes the meeple from the bag
    
    bool isMeepleInBag(const Meeple& meeple) const;         //returns, if this meeple is in the bag
    int getMeepleIndex(const Meeple& meeple) const;         //returns the index of the meeple in the bag; returns -1, if the meeple is not in the bag


};

