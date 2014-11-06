#include "MeepleBag.h"


MeepleBag::MeepleBag(MeepleColor::Enum color){       //creates a new bag with 8 brand new meeples
    meeples.push_back(new Meeple(color, MeepleSize::SMALL, MeepleShape::SQUARE, MeepleDetail::NO_HOLE));
    meeples.push_back(new Meeple(color, MeepleSize::SMALL, MeepleShape::SQUARE, MeepleDetail::HOLE));
    meeples.push_back(new Meeple(color, MeepleSize::SMALL, MeepleShape::ROUND, MeepleDetail::NO_HOLE));
    meeples.push_back(new Meeple(color, MeepleSize::SMALL, MeepleShape::ROUND, MeepleDetail::HOLE));
    meeples.push_back(new Meeple(color, MeepleSize::BIG, MeepleShape::SQUARE, MeepleDetail::NO_HOLE));
    meeples.push_back(new Meeple(color, MeepleSize::BIG, MeepleShape::SQUARE, MeepleDetail::HOLE));
    meeples.push_back(new Meeple(color, MeepleSize::BIG, MeepleShape::ROUND, MeepleDetail::NO_HOLE));
    meeples.push_back(new Meeple(color, MeepleSize::BIG, MeepleShape::ROUND, MeepleDetail::HOLE));
}

MeepleBag::~MeepleBag(){
    for (std::vector<Meeple*>::iterator iter = meeples.begin(); iter != meeples.end(); iter++){
        delete *(iter);
    }
    for (std::vector<Meeple*>::iterator iter = usedMeeples.begin(); iter != usedMeeples.end(); iter++){
        delete *(iter);
    }
} 
 
void MeepleBag::reset(){    
    meeples.insert(meeples.end(), usedMeeples.begin(), usedMeeples.end());
    usedMeeples.clear();
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
    throw new std::exception("unable to remove meeple from bag");
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

unsigned int MeepleBag::getSimilarMeepleCount(MeepleColor::Enum color) const{
    unsigned int count = 0;
    for (std::vector<Meeple*>::const_iterator it = meeples.begin(); it != meeples.end(); ++it){
        if ((*it)->getColor() == color){
            count++;
        }
    }
    return count;
}

unsigned int MeepleBag::getSimilarMeepleCount(MeepleSize::Enum size) const{
    unsigned int count = 0;
    for (std::vector<Meeple*>::const_iterator it = meeples.begin(); it != meeples.end(); ++it){
        if ((*it)->getSize() == size){
            count++;
        }
    }
    return count;
}

unsigned int MeepleBag::getSimilarMeepleCount(MeepleShape::Enum shape) const{
    unsigned int count = 0;
    for (std::vector<Meeple*>::const_iterator it = meeples.begin(); it != meeples.end(); ++it){
        if ((*it)->getShape() == shape){
            count++;
        }
    }
    return count;
}

unsigned int MeepleBag::getSimilarMeepleCount(MeepleDetail::Enum detail) const{
    unsigned int count = 0;
    for (std::vector<Meeple*>::const_iterator it = meeples.begin(); it != meeples.end(); ++it){
        if ((*it)->getDetail() == detail){
            count++;
        }
    }
    return count;
}