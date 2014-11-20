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
    explicit MeepleBag(const MeepleBag& base);              //Generates a new copy of this bag
    virtual ~MeepleBag();
    void reset();                                           //Reinitialises the object
   
    //This method must only be used in the copy-contructor of the board to generate a correct replica with correct meeple-pointer. That's the reason for the strange const-combination. THIS IS CORRECT AND INTENDED.
        Meeple* getUsedMeepleRepresentation(const Meeple& original) const;      //Returns the meeple which has the same porperties as the given one (ONLY SEARCHES IN THE USED MEEPLES) (the original meeple doesn't need to be in this bag). If no meeple is found, nullptr is returned

    unsigned int getMeepleCount() const;                    //returns the number of meeples in the bag
    const Meeple* getMeeple(unsigned int index) const;      //Returns a specific meeple from the set


    unsigned int getSimilarMeepleCount(MeepleProperty prop) const;          //Returns the number of meeples with the same property value
            
    Meeple* removeMeeple(const Meeple& meeple);             //removes the meeple from the bag
    Meeple* removeMeeple(unsigned int index);               //removes the meeple from the bag
    
    bool isMeepleInBag(const Meeple& meeple) const;         //returns, if this meeple is in the bag
    int getMeepleIndex(const Meeple& meeple) const;         //returns the index of the meeple in the bag; returns -1, if the meeple is not in the bag

    MeepleColor::Enum getBagColor() const;                  //returns the color of the meeples within the bag
};

