#pragma once

class MeepleBag;
class Board;

struct GameWinner{
	enum Enum{
		PLAYER_1,
		PLAYER_2,
		TIE
	};
};


//This data is given to the players/AIs, to calculate their next moves
//Note: this class doesn't hide its members, they are public and not-const. Care about not-const GameStates!
class GameState{
private:
    bool cloned;                    //if true, the struct has been generated with clone(), and the members need to be deleted()
public:
	MeepleBag* ownBag;          //The meeples which are owned by the AI
	MeepleBag* opponentBag;     //The meeples which are owned by the oppnent's AI
	Board* board;               //The current state of the board (and the meeples on the board)
    
    GameState(MeepleBag* ownBag, MeepleBag* opponentBag, Board* board);         //Doesn't copy the values - asigns them
    GameState(const GameState& base);         //Returns a copy of the gameState; Needs to be deleted()
    ~GameState();
};
