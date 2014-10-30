#include "meeple.h"





Meeple::Meeple(MeepleColor color, MeepleSize size, MeepleShape shape, MeepleDetail detail){
    this->color = color;
    this->size = size;
    this->shape = shape;
    this->detail = detail;
}

MeepleColor Meeple::getColor() const{
    return color;
}

MeepleSize Meeple::getSize() const{
    return size;
}

MeepleShape Meeple::getShape() const{
    return shape;
}

MeepleDetail Meeple::getDetail() const{
    return detail;
}

bool Meeple::equals(Meeple& meeple) const{
    if (meeple.getColor() == color &&
        meeple.getSize() == size &&
        meeple.getShape() == shape &&
        meeple.getDetail() == detail){
        return true;
    }
    return false;
}