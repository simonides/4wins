#include "MeepleBag.h"
#include <algorithm>
#include <stdint.h>


MeepleBag::MeepleBag(MeepleColor::Enum color){       //creates a new bag with 8 brand new meeples
    meeples.push_back(new Meeple(color, MeepleSize::SMALL, MeepleShape::SQUARE, MeepleDetail::NO_HOLE));
    meeples.push_back(new Meeple(color, MeepleSize::SMALL, MeepleShape::SQUARE, MeepleDetail::HOLE));
    meeples.push_back(new Meeple(color, MeepleSize::SMALL, MeepleShape::ROUND, MeepleDetail::NO_HOLE));
    meeples.push_back(new Meeple(color, MeepleSize::SMALL, MeepleShape::ROUND, MeepleDetail::HOLE));
    meeples.push_back(new Meeple(color, MeepleSize::BIG, MeepleShape::SQUARE, MeepleDetail::NO_HOLE));
    meeples.push_back(new Meeple(color, MeepleSize::BIG, MeepleShape::SQUARE, MeepleDetail::HOLE));
    meeples.push_back(new Meeple(color, MeepleSize::BIG, MeepleShape::ROUND, MeepleDetail::NO_HOLE));
    meeples.push_back(new Meeple(color, MeepleSize::BIG, MeepleShape::ROUND, MeepleDetail::HOLE));
    std::random_shuffle(meeples.begin(), meeples.end());
}

MeepleBag::~MeepleBag(){
    for (std::vector<Meeple*>::iterator it = meeples.begin(); it != meeples.end(); ++it){
        delete *(it);
    }
    for (std::vector<Meeple*>::iterator it = usedMeeples.begin(); it != usedMeeples.end(); ++it){
        delete *(it);
    }
} 

Meeple* MeepleBag::getUsedMeepleRepresentation(const Meeple& original) const{
    for (std::vector<Meeple*>::const_iterator it = usedMeeples.begin(); it != usedMeeples.end(); ++it){
        if (**it == original){
            return *it;
        }
    }
    return nullptr;
}
 
MeepleBag::MeepleBag(const MeepleBag& base){
    for (std::vector<Meeple*>::const_iterator it = base.meeples.begin(); it != base.meeples.end(); ++it){
        meeples.push_back(new Meeple(**it));
    }
    for (std::vector<Meeple*>::const_iterator it = base.usedMeeples.begin(); it != base.usedMeeples.end(); ++it){
        usedMeeples.push_back(new Meeple(**it));
    }
}



void MeepleBag::reset(){
    meeples.insert(meeples.end(), usedMeeples.begin(), usedMeeples.end());
    usedMeeples.clear();
    std::random_shuffle(meeples.begin(), meeples.end());
    assert(meeples.size() == 8); 
}

const Meeple* MeepleBag::getMeeple(unsigned int index) const{
    return meeples.at(index);
}

unsigned int MeepleBag::getMeepleCount() const{
    return meeples.size();
}

Meeple* MeepleBag::removeMeeple(const Meeple& meeple){
    for (std::vector<Meeple*>::iterator it = meeples.begin(); it != meeples.end(); ++it){
        if (meeple == **it){           
            Meeple* m = *it;
            meeples.erase(it);
            usedMeeples.push_back(m);
            return m;
        }
    }
    assert(false);
    return nullptr;
}

Meeple* MeepleBag::removeMeeple(unsigned int index){
    std::vector<Meeple*>::iterator it = meeples.begin() + index;
    Meeple* m = *it;
    meeples.erase(it);
    usedMeeples.push_back(m);
    return m;
}

bool MeepleBag::isMeepleInBag(const Meeple& meeple) const{
    for (std::vector<Meeple*>::const_iterator it = meeples.begin(); it != meeples.end(); ++it){
        if (meeple == **it){
            return true;
        }
    }
    return false;
}

int MeepleBag::getMeepleIndex(const Meeple& meeple) const{
    unsigned int index = 0;
    for (std::vector<Meeple*>::const_iterator it = meeples.begin(); it != meeples.end(); ++it, ++index){
        if (meeple == **it){
            return index;
        }
    }
    return -1;
}

unsigned int MeepleBag::getSimilarMeepleCount(MeepleProperty prop) const{
    unsigned int count = 0;
    for (std::vector<Meeple*>::const_iterator it = meeples.begin(); it != meeples.end(); ++it){
        if ((*it)->hasSameProperty(prop)){
            count++;
        }
    }
    return count;
}

MeepleColor::Enum MeepleBag::getBagColor() const{
    std::vector<Meeple*>::const_iterator it = meeples.begin();
    if (it != meeples.end()){
        return (*it)->getColor();
    }
    return (*usedMeeples.begin())->getColor();
}