#include "Meeple.h"





Meeple::Meeple(MeepleColor::Enum color, MeepleSize::Enum size, MeepleShape::Enum shape, MeepleDetail::Enum detail){
    this->color = color;
    this->size = size;
    this->shape = shape;
    this->detail = detail;
}

MeepleColor::Enum Meeple::getColor() const{
    return color;
}

MeepleSize::Enum Meeple::getSize() const{
    return size;
}

MeepleShape::Enum Meeple::getShape() const{
    return shape;
}

MeepleDetail::Enum Meeple::getDetail() const{
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