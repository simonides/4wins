#pragma once

#include <SFML/Graphics.hpp>
//#include <list>

#include "RMeeple.h"
#include "Board.h"

class MeepleBag;
//class Board;
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
    const MeepleBag* ownBag;            //The meeples which are owned by the AI
    const MeepleBag* opponentBag;       //The meeples which are owned by the oppnent's AI
    const Board* board;                 //The current state of the board (and the meeples on the board)
};


//Contains all information about a game, and handles the game loop
//A Game-object is usable for exacly one round
class Game
{

private:
	sf::RenderWindow* window;
	
	sf::RectangleShape leftPanel;
	sf::RectangleShape rightPanel;
	sf::RectangleShape boardPanel;

	std::vector<sf::CircleShape*> fields;

	
	std::vector<RMeeple*> meeples;
	sf::Vector2f mousePosRelativeToMeepleBoundary;
	//sf::Vector2f lastValidPosition;

	sf::Texture meepleSprite;
	sf::Texture glowSprite;

	sf::Texture boardTexture;
    

	MeepleBag* bag[2];
    Board board;

    GameState gameStatePlayer1;                //stores the gamestate for player 1 (buffered)
    GameState gameStatePlayer2;
        
    I_Player* player1;
    I_Player* player2;


    void runGameCycle(I_Player* player, I_Player* opponent, GameState& gameStateForPlayer, GameState& gameStateForOpponent, int playerNr);
	void setUp();
	void initFields();
	void initMeeples();
	void loadTextures();
	sf::IntRect getTextRectForPos(const Meeple& meeple);
	sf::IntRect getGlowTextRectForPos(const Meeple& meeple);

public:
    Game(sf::RenderWindow& window, I_Player& player1, I_Player& player2); //Initialises the game with 2 players
	virtual ~Game();
	void reset();                               //Reinitialises the object

    GameWinner::Enum runGame();                 //Runs the game, until it is over; returns the winner
  
	void pollEvents();


};

