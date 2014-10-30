#pragma once

#include "helper.h"



struct MeepleColor{
    enum Enum{
        WHITE,
        BLACK
    };
};

struct MeepleSize{
    enum Enum{
        BIG,
        SMALL
    };
};

struct MeepleShape{
    enum Enum{
        ROUND,
        SQUARE
    };
};

struct MeepleDetail{
    enum Enum{
        HOLE,
        NO_HOLE
    };
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
    
    MeepleColor::Enum getColor() const;
    MeepleSize::Enum getSize() const;
    MeepleShape::Enum getShape() const;
    MeepleDetail::Enum getDetail() const;

    bool equals(Meeple& meeple) const;

};

