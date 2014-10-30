#include "MeepleBag.h"



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
} 
 


std::set<Meeple*>::const_iterator MeepleBag::getMeeples() const{
     return meeples.begin();
}

void MeepleBag::removeMeeple(Meeple& meeple){
    for (std::set<Meeple*>::iterator iter = meeples.begin(); iter != meeples.end(); iter++){
        if (meeple.equals(**iter)){
            meeples.erase(iter);
            delete *(iter);
            return;
        }
    }
    throw new std::exception("unable to remove meeple from bag");
}

bool MeepleBag::isMeepleInBag(Meeple& meeple) const{
    for (std::set<Meeple*>::iterator iter = meeples.begin(); iter != meeples.end(); iter++){
        if (meeple.equals(**iter)){
            return true;
        }
    }
    return false;
}