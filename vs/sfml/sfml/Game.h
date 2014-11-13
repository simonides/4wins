#pragma once

#include <SFML/Graphics.hpp>
class MeepleBag;
class Board;
class I_Player;


struct GameWinner{
    enum Enum{
        PLAYER_1,
        PLAYER_2,
        TIE
    };
};


//This data is given to the players/AIs, to calculate their next moves
struct GameState{              
    const MeepleBag& ownBag;            //The meeples which are owned by the AI
    const MeepleBag& opponentBag;       //The meeples which are owned by the oppnent's AI
    const Board& board;                 //The current state of the board (and the meeples on the board)
    GameState(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Board& board) : ownBag(ownBag), opponentBag(opponentBag), board(board){}     //needed to initialise the referneces
};


//Contains all information about a game, and handles the game loop
//A Game-object is usable for exacly one round
class Game
{
private:
	sf::RenderWindow* window;

    MeepleBag* bag[2];
    Board* board;

    GameState* gameStatePlayer1;                //stores the gamestate for player 1 (buffered)
    GameState* gameStatePlayer2;

    I_Player& player1;
    I_Player& player2;

    void runGameCycle(I_Player& player, I_Player& opponent, GameState& gameStateForPlayer, GameState& gameStateForOpponent, int playerNr);
public:
    Game(sf::RenderWindow* window, I_Player* player1, I_Player* player2); //Initialises the game with 2 players
	virtual ~Game();
	void reset();                               //Reinitialises the object

    GameWinner::Enum runGame();                 //Runs the game, until it is over; returns the winner
  
	void pollEvents();
};

