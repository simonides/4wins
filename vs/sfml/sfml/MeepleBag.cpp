#include "MeepleBag.h"

//#include <string>
//#include <iostream>


MeepleBag::MeepleBag(MeepleColor::Enum color){       //creates a new bag with 8 brand new meeples
    meeples.insert(new Meeple(color, MeepleSize::SMALL, MeepleShape::SQUARE, MeepleDetail::NO_HOLE));
    meeples.insert(new Meeple(color, MeepleSize::SMALL, MeepleShape::SQUARE, MeepleDetail::HOLE));
    meeples.insert(new Meeple(color, MeepleSize::SMALL, MeepleShape::ROUND,  MeepleDetail::NO_HOLE));
    meeples.insert(new Meeple(color, MeepleSize::SMALL, MeepleShape::ROUND,  MeepleDetail::HOLE));
    meeples.insert(new Meeple(color, MeepleSize::BIG,   MeepleShape::SQUARE, MeepleDetail::NO_HOLE));
    meeples.insert(new Meeple(color, MeepleSize::BIG,   MeepleShape::SQUARE, MeepleDetail::HOLE));
    meeples.insert(new Meeple(color, MeepleSize::BIG,   MeepleShape::ROUND,  MeepleDetail::NO_HOLE));
    meeples.insert(new Meeple(color, MeepleSize::BIG,   MeepleShape::ROUND,  MeepleDetail::HOLE));
}


MeepleBag::~MeepleBag(){
    for (std::set<Meeple*>::iterator iter = meeples.begin(); iter != meeples.end(); iter++){
        delete *(iter);
    }
    for (std::set<Meeple*>::iterator iter = usedMeeples.begin(); iter != usedMeeples.end(); iter++){
        delete *(iter);
    }
} 
 

void MeepleBag::reset(){
    meeples.insert(usedMeeples.begin(), usedMeeples.end());
    usedMeeples.clear();
}

const std::set<const Meeple*>* MeepleBag::getMeeples() const{
    //TODO: how to cast this set with c++ casts? reinterpret_cast is propably the wrong method
    return (std::set<const Meeple*>*)&meeples;
}


unsigned int MeepleBag::getMeepleCount() const{
    return meeples.size();
}

Meeple* MeepleBag::removeMeeple(const Meeple& meeple){
    for (std::set<Meeple*>::iterator iter = meeples.begin(); iter != meeples.end(); iter++){              
        if (meeple == **iter){           
            Meeple* m = *iter;
            meeples.erase(iter);
            usedMeeples.insert(m);
            return m;
        }
    }
    throw new std::exception("unable to remove meeple from bag");
}

bool MeepleBag::isMeepleInBag(const Meeple& meeple) const{
    for (std::set<Meeple*>::iterator iter = meeples.begin(); iter != meeples.end(); iter++){
        if (meeple == **iter){
            return true;
        }
    }
    return false;
}

unsigned int MeepleBag::getSimilarMeepleCount(MeepleColor::Enum color) const{
    unsigned int count = 0;
    for (std::set<Meeple*>::iterator iter = meeples.begin(); iter != meeples.end(); iter++){
        if ((*iter)->getColor() == color){
            count++;
        }
    }
    return count;
}

unsigned int MeepleBag::getSimilarMeepleCount(MeepleSize::Enum size) const{
    unsigned int count = 0;
    for (std::set<Meeple*>::iterator iter = meeples.begin(); iter != meeples.end(); iter++){
        if ((*iter)->getSize() == size){
            count++;
        }
    }
    return count;
}

unsigned int MeepleBag::getSimilarMeepleCount(MeepleShape::Enum shape) const{
    unsigned int count = 0;
    for (std::set<Meeple*>::iterator iter = meeples.begin(); iter != meeples.end(); iter++){
        if ((*iter)->getShape() == shape){
            count++;
        }
    }
    return count;
}

unsigned int MeepleBag::getSimilarMeepleCount(MeepleDetail::Enum detail) const{
    unsigned int count = 0;
    for (std::set<Meeple*>::iterator iter = meeples.begin(); iter != meeples.end(); iter++){
        if ((*iter)->getDetail() == detail){
            count++;
        }
    }
    return count;
}