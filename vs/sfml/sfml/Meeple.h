#pragma once

#include "helper.h"
#include <ostream>
#include <assert.h>



struct MeepleColor{
    enum Enum{
        WHITE,
        BLACK
    };
    static std::string toString(Enum color);
};
struct MeepleSize{
    enum Enum{
        BIG,
        SMALL
    };
    static std::string toString(Enum size);
};
struct MeepleShape{
    enum Enum{
        ROUND,
        SQUARE
    };
    static std::string toString(Enum shape);
};
struct MeepleDetail{
    enum Enum{
        HOLE,
        NO_HOLE
    };
    static std::string toString(Enum detail);
};


//This struct represents either a color, or a size, or a shape, or a detail (can be considered as some kind of base-struct)
struct MeepleProperty{
    enum Type{
        MEEPLE_COLOR,
        MEEPLE_SIZE,
        MEEPLE_SHAPE,
        MEEPLE_DETAIL
    } type;
    
    union {
        MeepleColor::Enum color;
        MeepleSize::Enum size;
        MeepleShape::Enum shape;
        MeepleDetail::Enum detail;
    } value;
    
    bool operator == (const MeepleProperty& prop);
    bool operator != (const MeepleProperty& prop);
};



//Spielfigur
class Meeple{

private:
    MeepleColor::Enum color;
    MeepleSize::Enum size;
    MeepleShape::Enum shape;
    MeepleDetail::Enum detail;

public:
    Meeple(MeepleColor::Enum color, MeepleSize::Enum size, MeepleShape::Enum shape, MeepleDetail::Enum detail);
    explicit Meeple(const Meeple& base);
   
    MeepleColor::Enum getColor() const;
    MeepleSize::Enum getSize() const;
    MeepleShape::Enum getShape() const;
    MeepleDetail::Enum getDetail() const;

    MeepleProperty getProperty(MeepleProperty::Type type) const;   
    MeepleProperty::Type getPropertyType(unsigned int propIdx) const;       //Returns the type, which is represented by this index
    unsigned int getIndexFprPropertyType(MeepleProperty::Type type) const;  //Returns the index, which can be used to retrieve the property
    MeepleProperty getProperty(unsigned int propIdx) const;                 //To be able to iterate through the properties
    
    bool hasSameProperty(MeepleProperty prop) const;      //Checks, if this meeple is similar (same property)


    std::string toString() const;
      
    bool equals(const Meeple& meeple) const;
    bool operator == (const Meeple& meeple) const;
};

