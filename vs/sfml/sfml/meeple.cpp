#include "Meeple.h"

#include <iostream>


Meeple::Meeple(MeepleColor::Enum color, MeepleSize::Enum size, MeepleShape::Enum shape, MeepleDetail::Enum detail){
    this->color = color;
    this->size = size;
    this->shape = shape;
    this->detail = detail;
}

MeepleProperty Meeple::getProperty(MeepleProperty::Type type) const{
    MeepleProperty property;
    property.type = type;
    switch (type){
        case MeepleProperty::MEEPLE_COLOR:  property.value.color = color;   break;
        case MeepleProperty::MEEPLE_SIZE:   property.value.size = size;     break;
        case MeepleProperty::MEEPLE_SHAPE:  property.value.shape = shape;   break;
        case MeepleProperty::MEEPLE_DETAIL: property.value.detail = detail; break;
    }
    return property;
}

MeepleProperty::Type Meeple::getPropertyType(unsigned int propIdx) const{
    assert(propIdx < 4);
    switch (propIdx){
        case 0: return MeepleProperty::MEEPLE_COLOR;
        case 1: return MeepleProperty::MEEPLE_SIZE;
        case 2: return MeepleProperty::MEEPLE_SHAPE;
        case 3: return MeepleProperty::MEEPLE_DETAIL;
    }
}

unsigned int Meeple::getIndexFprPropertyType(MeepleProperty::Type type) const{
    switch (type){
        case MeepleProperty::MEEPLE_COLOR:  return 0;
        case MeepleProperty::MEEPLE_SIZE:   return 1;
        case MeepleProperty::MEEPLE_SHAPE:  return 2;
        case MeepleProperty::MEEPLE_DETAIL: return 3;
    }
}

MeepleProperty Meeple::getProperty(unsigned int propIdx) const{
    return getProperty(getPropertyType(propIdx));
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

bool Meeple::hasSameProperty(MeepleProperty prop) const{
    switch (prop.type){
        case MeepleProperty::MEEPLE_COLOR:  return color == prop.value.color;
        case MeepleProperty::MEEPLE_SIZE:   return size == prop.value.size;
        case MeepleProperty::MEEPLE_SHAPE:  return shape == prop.value.shape;
        case MeepleProperty::MEEPLE_DETAIL: return detail == prop.value.detail;
    }
    throw new std::exception("unable to compare");
}


std::string Meeple::toString() const{
    return MeepleColor::toString(color) + ' ' + MeepleSize::toString(size) + ' ' + MeepleShape::toString(shape) + ' ' + MeepleDetail::toString(detail);
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

bool Meeple::operator == (Meeple& meeple) const{
    return equals(meeple);
}