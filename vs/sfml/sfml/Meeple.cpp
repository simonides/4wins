#include "Meeple.h"

#include <iostream>



std::string MeepleColor::toString(Enum color){
    return color == WHITE ? "white" : "black";
}

std::string MeepleSize::toString(Enum size){
    return size == BIG ? "big" : "small";
}

std::string MeepleShape::toString(Enum shape){
    return shape == ROUND ? "round" : "square";
}

std::string MeepleDetail::toString(Enum detail){
    return detail == HOLE ? "hole" : "solid";
}


bool MeepleProperty::operator == (const MeepleProperty& prop){
    assert(type == prop.type);
    switch (type){
        case MEEPLE_COLOR:	return value.color == prop.value.color;
        case MEEPLE_SIZE:	return value.size == prop.value.size;
        case MEEPLE_SHAPE:	return value.shape == prop.value.shape;
        case MEEPLE_DETAIL:	return value.detail == prop.value.detail;
    }
    throw new std::exception("unable to compare");
}

bool MeepleProperty::operator != (const MeepleProperty& prop){
    assert(type == prop.type);
    return !(*this == prop);
}




Meeple::Meeple(MeepleColor::Enum color, MeepleSize::Enum size, MeepleShape::Enum shape, MeepleDetail::Enum detail) :
    color(color), size(size), shape(shape), detail(detail){
}

Meeple::Meeple(const Meeple& base) :
    color(base.color), size(base.size), shape(base.shape), detail(base.detail){
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
    throw new std::exception("unable to return type");
}

unsigned int Meeple::getIndexFprPropertyType(MeepleProperty::Type type) const{
    switch (type){
        case MeepleProperty::MEEPLE_COLOR:  return 0;
        case MeepleProperty::MEEPLE_SIZE:   return 1;
        case MeepleProperty::MEEPLE_SHAPE:  return 2;
        case MeepleProperty::MEEPLE_DETAIL: return 3;
    }
    throw new std::exception("unable to return index");
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

bool Meeple::equals(const Meeple& meeple) const{
    if (meeple.getColor() == color &&
        meeple.getSize() == size &&
        meeple.getShape() == shape &&
        meeple.getDetail() == detail){
        return true;
    }
    return false;
}

bool Meeple::operator == (const Meeple& meeple) const{
    return equals(meeple);
}