#pragma once
#include <cstdint>
#include <string>
#include <set>

class MeepleBag;
class Meeple;

struct BoardPos{
    uint8_t x;
    uint8_t y;
    std::string toString();
};

struct WinCombination{      //Contains 4 meeples, that are either in a row, column, or diagonal. If these meeples are similar, a player won the game
    const Meeple *meeples[4];
    BoardPos positions[4];   //Position of the meeples
    WinCombination();
    WinCombination(const WinCombination& base); //Creates a copy. (!) The meeple-pointers are set tu nullptr (!)
};

struct WinCombinationSet{   //Contains all possible combinations, that can lead to a victory
    std::set<WinCombination*> combination;
};


class Board{
private:
    Meeple* board[4][4];   

    mutable bool isWinCombinationSetUp2Date;                    //if false, the winCombinations-field needs to be updated before usage (can be changed by the const function "updateWinCombinations")
    WinCombinationSet winCombinations;                          //Contains all possible win combinations (is buffered, to avoid new's and delete's all the time)
    
    bool checkSimpleWinCombination(const WinCombination* comb) const;  //Checks 4 meeples for similarity
    void updateWinCombinations() const;                         //updates the winCombination-field (sets the meeple-pointers to the current board-state)
    explicit Board(const Board& base);
public:
    Board();
    Board(const Board& base, const MeepleBag* bag1, const MeepleBag* bag2);
    virtual ~Board();

    void reset();                                               //Reinitialises the object

    const Meeple* getMeeple(BoardPos position) const;
    bool isFieldEmpty(BoardPos position) const;
    void setMeeple(BoardPos position, Meeple& meeple);
    Meeple* removeMeeple(BoardPos position);                    //Removes a meeple from the board; DOES NOT DELETE the meeple
    const WinCombinationSet* getWinCombinations() const;        //Returns all possible win-combinations. The result must not be deleted
    BoardPos getRandomEmptyField() const;                       //Returns an random empty field

    bool isFull() const;
    
    const WinCombination* checkWinSituation() const;            //Checks, if there are 4 similar meeples in a row/col/diagonal; if yes: return the meeples from the combination; otherwise, return nullptr
    
    void print(std::ostream& output) const;                     //For debugging purposes only
};
