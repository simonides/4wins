#pragma once

class Meeple{
  
private:
    bool big;
    bool round;
    bool hole;

public:
    Meeple(bool isBig, bool isRound, bool hasHole);
       
    bool isBig() const;
    bool isRound() const;
    bool hasHole() const;
};

