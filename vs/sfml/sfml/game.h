#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>

#include "RMeeple.h"
#include "GameState.h"
#include "ResourceManager.h"
#include "RBackground.h"
#include "Board.h"
#include "ColorAnimation.h"
#include "helper.h"

class RBackground;
class ParticleBuilder;
class ParticleSystem;
class ThreadController;
class MeepleBag;
class I_Player;
class RBoard;
class Board;
class RBag;


struct GameMenuDecision{
    enum Enum{
        REPLAY,
        BACK_TO_MENU,
        EXIT_GAME,
        KEEP_PLAYING         //Game-internal, until the game ended and the player pressed a button
    };
};

struct Player{
	enum{
		HUMAN,
		I_PLAYER, //TODO: still not fully supported
		TC
	} type;
	
	ResourceManager::ResourceRect playerAvatar;  

	union{
		ThreadController* controller;
		I_Player* player;
	};
	RBag* rbag;
	MeepleBag* logicalMeepleBag;

    Interval meeplePositionThinkTime;   //For TC/I_Player only: how long the AI thinks about the meeple position
    Interval meepleChoosingThinkTime;   //For TC/I_Player only: how long the AI thinks about the meeple to choose
};



class Game
{
private:
	sf::RenderWindow* window;
	ResourceManager* resourceLoader;
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

    uint8_t activePlayerIndex;

    Board* logicalBoard;					
    RBoard* board;
    Player* players[2];
    GameState* gameStates[2];               //stores the gamestate for player 1 (buffered)




	//GameReturn runGameSwitch;
	bool pressedLeftMouse;
	bool releasedLeftMouse;
    bool rightMouseButtonPressed;

	sf::Vector2f mousePosRelativeToMeepleBoundary;
	sf::Vector2f convertedMousePos;
	sf::Vector2f lastValidPosition;

	RMeeple* rMeepleToSet;
	std::vector<RMeeple*> meeplesToDrawAndSort;


    

	    const sf::Color HOVERED_MEEPLE_GLOW_COLOR;
	    const sf::Color SELECTED_MEEPLE_GLOW_COLOR;
    
	//endscreen
	    sf::RectangleShape* hoveredButtonPtr;
	    sf::RectangleShape exitButton;
	    sf::RectangleShape restartButton;
	    sf::RectangleShape menuButton;
        const sf::Vector2f GAME_MENU_BUTTON_SIZE;
        const sf::Color GAME_MENU_BUTTON_COLOR;
        const sf::Color GAME_MENU_BUTTON_HOVER_COLOR;


	//Particles:
        ParticleSystem* particleSystem;                 //Particle controller: renders and simulates all particles
        ParticleBuilder* dustBuilder;                   //produces dust-particles
        ParticleBuilder* mouseCursorParticleBuilder;    //produces snow when mouse right-click
        ParticleBuilder* endScreenParticleBuilder;      //produces stars at the end of the game


    //loopvars
        const Meeple* meepleToSet;              //The meeple that has been selected by the Player (has to be set by the opponent). It's glow has the color SELECTED_MEEPLE_GLOW_COLOR
        RMeeple* hoveredMeeple;                 //Hovered meeple, while the Player chooses a meeple for the opponent
        BoardPos posMeepleTo;
        bool dragMeeple;
        bool firstFrameOfState;                 //Is set to true for exacly one frame, everytime the state changes

	//State functions:
	    LoopState i_playerSelectMeeple();
	    LoopState humanSelectMeeple();
	    LoopState tcStartSelectMeeple();
	    LoopState tcWaitForSelectedMeeple();
        LoopState highlightSelectedMeeple(float elapsedTime);

	    LoopState i_playerSelectMeeplePosition();
	    LoopState humanSelectMeeplePosition();

	    LoopState tcStartSelectMeeplePosition();
	    LoopState tcWaitForSelectedMeeplePosition();
        LoopState moveMeepleToSelectedPosition(float elapsedTime);
	
	    LoopState checkEndCondition();
        GameMenuDecision::Enum displayEndscreen(float elapsedTime);

    //moveMeepleToSelectedPosition - animation/interpolation
        sf::Vector2f initialPosition;
        sf::Vector2f targetPosition;
        
        float moveMeepleAnimationDistance;
        float moveMeepleAnimationProgress;
        float moveMeepleAnimationMaxLiftDistance;
        const float MOVE_MEEPLE_ANIMATION_SPEED;
        //const Interval MOVE_MEEPLE_ANIMATION_DROP_OFF_DISTANCE; //The distance 
        const Interval MOVE_MEEPLE_ANIMATION_MAX_LIFT_DISTANCE;     //max. y-distance between the meeple's route and the shortest path. Animated with a sinus
    
    //moveMeepleToSelectedPosition & highlightSelectedMeeple
        float remainingThinkTime;                       //Remaining time, until the animation starts



        
    //End-Screen: rainbow-animation  for winCombination
        RMeeple* winningCombiRMeeples[4];
        ColorAnimation colorAnimations[4];     //Animations for all meeples in the winCombination


	// other functions
	void switchPlayers();
	void initMeeples();
	//sf::Color rainbow(float progress) const; // TODO gehört raus :D
    void createMeepleDust(sf::FloatRect fieldBounds);
    void pollEvents();

    Game& operator = (const Game&);
public:
    Game(sf::RenderWindow& window, Player* players[2], ResourceManager& resourceLoader); //Initialises the game with 2 players
	virtual ~Game();
	void reset();                               //Reinitialises the object for another round
    GameMenuDecision::Enum runGame();           //Runs the game, until it is over; returns the winner

};

