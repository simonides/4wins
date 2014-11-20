#pragma once
#include <SFML/Graphics.hpp>

#include "RMeeple.h"
#include "GameState.h"

#include <cstdint>
#include <vector>

class ThreadController;
class MeepleBag;
class I_Player;
class RBoard;
class Board;
class RBag;

struct Player{
	enum{
		HUMAN,
		I_PLAYER,
		TC
	} type;
	
	union{
		ThreadController* controller;
		I_Player* player;
	};
	RBag* rbag;
	MeepleBag* logicalMeepleBag;
};


//Contains all information about a game, and handles the game loop
//A Game-object is usable for exacly one round
class Game
{
private:



	sf::RenderWindow* window;

	sf::Vector2f mousePosRelativeToMeepleBoundary;

	sf::Texture meepleSprite;
	sf::Texture glowSprite;
	sf::Texture boardTexture;
	sf::Texture fieldTexture;
	sf::Texture fieldTextureOccupied;
	sf::Font font;

	sf::Vector2f convertedMousePos;
	sf::Vector2f lastValidPosition;
	bool pressedLeftMouse;
	bool releasedLeftMouse;

	Board* logicalBoard;
	RBoard* board;

	Player* players[2];
    GameState gameStatePlayer[2];                //stores the gamestate for player 1 (buffered)
	
	uint8_t activePlayerIndex;

	sf::Clock meepleGlowAnimationClock;
	sf::RectangleShape endscreenPanel;
	RMeeple* winningCombiRMeeples[4];
	bool drawEndScreen;
	float color4MGlow[4];

	void switchPlayers();
	void initMeeples();
	void loadTextures();

	sf::Color rainbow(float progress) const ;

	RMeeple* rMeepleToSet;




public:
    Game(sf::RenderWindow& window, Player& player1, Player& player2); //Initialises the game with 2 players
	virtual ~Game();
	void reset();                               //Reinitialises the object

    GameWinner::Enum runGame();                 //Runs the game, until it is over; returns the winner
  
	void pollEvents();
};

