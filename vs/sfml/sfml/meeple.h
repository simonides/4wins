#pragma once

#include "helper.h"
#include <ostream>


struct MeepleColor{
    enum Enum{
        WHITE,
        BLACK
    };
    static std::string toString(Enum color){
        return color == WHITE ? "white" : "black";
    }
};

struct MeepleSize{
    enum Enum{
        BIG,
        SMALL
    };
    static std::string toString(Enum size){
        return size == BIG ? "big" : "small";
    }
};

struct MeepleShape{
    enum Enum{
        ROUND,
        SQUARE
    };
    static std::string toString(Enum shape){
        return shape == ROUND ? "round" : "square";
    }
};

struct MeepleDetail{
    enum Enum{
        HOLE,
        NO_HOLE
    };
    static std::string toString(Enum detail){
        return detail == HOLE ? "hole" : "solid";
    }
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

    std::string toString() const;


    void doSth();



    bool equals(Meeple& meeple) const;
    bool operator == (Meeple& meeple) const;
};

