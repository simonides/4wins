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
	sf::Texture backgroundTexture;
	sf::Texture exitButtonTexture;
	sf::Texture restartButtonTexture;
	sf::Texture particleSprites;

	sf::RectangleShape exitButton;
	sf::RectangleShape restartButton;
	sf::Color buttonColor;

	sf::RectangleShape* hoveredButtonPtr;


	sf::RectangleShape background;
	std::vector<RMeeple*> meeplesToDrawAndSort;

	sf::Font font;

	sf::Vector2f convertedMousePos;
	sf::Vector2f lastValidPosition;
	bool pressedLeftMouse;
	bool releasedLeftMouse;

	Board* logicalBoard;
	RBoard* board;

	Player* players[2];
    GameState* gameStates[2];                //stores the gamestate for player 1 (buffered)
	
	uint8_t activePlayerIndex;

	sf::Clock meepleGlowAnimationClock;
	RMeeple* winningCombiRMeeples[4];
	bool drawEndScreen;
	bool runGameSwitch;
	float color4MGlow[4];

	void switchPlayers();
	void initMeeples();
	void loadTextures();
	void setString(std::string message);
	sf::Color rainbow(float progress) const ;

	RMeeple* rMeepleToSet;
	sf::Text text;
    
public:
    Game(sf::RenderWindow& window, Player& player1, Player& player2); //Initialises the game with 2 players
	virtual ~Game();
	void reset();                               //Reinitialises the object

    void runGame();                 //Runs the game, until it is over; returns the winner
  
	void pollEvents();
};

