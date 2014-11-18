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
struct GameState{
	const MeepleBag* ownBag;            //The meeples which are owned by the AI
	const MeepleBag* opponentBag;       //The meeples which are owned by the oppnent's AI
	const Board* board;                 //The current state of the board (and the meeples on the board)
};