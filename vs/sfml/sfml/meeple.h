#pragma once

#include "helper.h"

//Spielfigur
class Meeple{
  
private:
    MeepleColor color;
    MeepleSize size;
    MeepleShape shape;
    MeepleDetail detail;

public:
    Meeple(MeepleColor color, MeepleSize size, MeepleShape shape, MeepleDetail detail);
    
    MeepleColor getColor() const;
    MeepleSize getSize() const;
    MeepleShape getShape() const;
    MeepleDetail getDetail() const;

    bool equals(Meeple& meeple) const;

};

