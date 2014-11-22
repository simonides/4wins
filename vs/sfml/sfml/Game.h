#pragma once
#include <SFML/Graphics.hpp>

#include "RMeeple.h"
#include "GameState.h"
#include "ResourceLoader.h"
#include "RBackground.h"

#include <cstdint>
#include <vector>
#include "Board.h"
#include "ColorAnimation.h"

class RBackground;
class ParticleBuilder;
class ParticleSystem;
class ThreadController;
class MeepleBag;
class I_Player;
class RBoard;
class Board;
class RBag;
//class ResourceLoader;

enum GameReturn{
	REPLAY, 
    BACK_TO_MENU, 
    EXIT_GAME,
    KEEP_PLAYING         //Game-internal, until the game ended and the player pressed a button
};

struct Player{
	enum{
		HUMAN,
		I_PLAYER, //TODO: still not fully supported
		TC
	} type;
	
	ResourceLoader::ResourceRect playerAvatar;

	union{
		ThreadController* controller;
		I_Player* player;
	};
	RBag* rbag;
	MeepleBag* logicalMeepleBag;
};



class Game
{
private:

	sf::RenderWindow* window;
	ResourceLoader* resourceLoader;
	RBackground* background;

	//gameloop
	enum LoopState{
		INIT_STATE,
		//Select Meeple for opponent
		I_PLAYER_SELECT_MEEPLE,
		HUMAN_SELECT_MEEPLE,
		TC_START_SELECT_MEEPLE, TC_WAIT_FOR_SELECTED_MEEPLE, HIGHLIGHT_SELECTED_MEEPLE,
		//Select Meeple position
		I_PLAYER_SELECT_MEEPLE_POSITION,
		HUMAN_SELECT_MEEPLE_POSITION,
		TC_START_SELECT_MEEPLE_POSITION, TC_WAIT_FOR_SELECTED_MEEPLE_POSITION, MOVE_MEEPLE_TO_SELECTED_POSITION,
		//check win /tie 
		CHECK_END_CONDITION, DISPLAY_END_SCREEN
	};

	//GameReturn runGameSwitch;
	bool pressedLeftMouse;
	bool releasedLeftMouse;
    bool rightMouseButtonPressed;

	sf::Vector2f mousePosRelativeToMeepleBoundary;
	sf::Vector2f convertedMousePos;
	sf::Vector2f lastValidPosition;

	RMeeple* rMeepleToSet;
	std::vector<RMeeple*> meeplesToDrawAndSort;

	uint8_t activePlayerIndex;

	Board* logicalBoard;					//todo needed????
	RBoard* board;
	Player* players[2];
	GameState* gameStates[2];               //stores the gamestate for player 1 (buffered)



	//loopvars
	const Meeple* meepleToSet;
	RMeeple* glowMeepleTmp;
	BoardPos posMeepleTo;
	bool dragMeeple;
    ParticleSystem* particleSystem;
    ParticleBuilder* dustBuilder;
    ParticleBuilder* mouseCursorParticleBuilder;
	ParticleBuilder* endScreenParticleBuilder;
	sf::Color STANDARD_GLOW;
	sf::Color SELECTED_GLOW;

	//endscreen
	bool drawEndScreen;
	sf::RectangleShape* hoveredButtonPtr;
	sf::RectangleShape exitButton;
	sf::RectangleShape restartButton;
	sf::RectangleShape menuButton;
	sf::Color buttonColor;
	//float color4MGlow[4];
	
	//sf::Clock meepleGlowAnimationClock;
	
	

	// loop functions
	LoopState i_playerSelectMeeple();
	LoopState humanSelectMeeple();
	LoopState tcStartSelectMeeple();
	LoopState tcWaitForSelectedMeeple();
	LoopState highlightSelectedMeeple();

	LoopState i_playerSelectMeeplePosition();
	LoopState humanSelectMeeplePosition();

	LoopState tcStartSelectMeeplePosition();
	LoopState tcWaitForSelectedMeeplePosition();
	LoopState MoveMeepleToSelectedPosition();
	
	LoopState checkEndCondition();
    GameReturn displayEndscreen(float elapsedTime);

    //End-Screen: rainbow-animation  for winCombination
        RMeeple* winningCombiRMeeples[4];
        ColorAnimation colorAnimations[4];     //Animations for all meeples in the winCombination


	// other functions
	void switchPlayers();
	void initMeeples();
	sf::Color rainbow(float progress) const; // TODO gehört raus :D
    void createMeepleDust(sf::FloatRect fieldBounds);

public:
    Game(sf::RenderWindow& window, Player* players[2], ResourceLoader& resourceLoader); //Initialises the game with 2 players
	virtual ~Game();
	void reset();                               //Reinitialises the object
    GameReturn runGame();                 //Runs the game, until it is over; returns the winner
	void pollEvents();
};

