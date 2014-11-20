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
class GameState{
private:
    bool cloned;                    //if true, the struct has been generated with clone(), and the members need to be deleted()
public:
	const MeepleBag* ownBag;          //The meeples which are owned by the AI
	const MeepleBag* opponentBag;     //The meeples which are owned by the oppnent's AI
	const Board* board;               //The current state of the board (and the meeples on the board)
    
    GameState(const MeepleBag* ownBag, const MeepleBag* opponentBag, const Board* board);         //Doesn't copy the values - asigns them
    GameState(const GameState& base);         //Returns a copy of the gameState; Needs to be deleted()
    ~GameState();
};
