#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>

class ResourceManager;
class SoundManager;

class PreMenu
{
public:
	PreMenu(sf::RenderWindow& window, ResourceManager& resourceManager, SoundManager& soundManager);
	void runLoop();

private:
	void pollEvents();
	bool crossfade(float elapsedTime, sf::RectangleShape& rect1, sf::RectangleShape& rect2);
	void skipToMenu();

	PreMenu& operator=(PreMenu&);

	enum
	{
		CROSSFADE, WAIT,
	} loopState;

	bool firstFadingFrame;
	uint8_t fadeIndexA;
	uint8_t fadeIndexB;

	const float ANIMATION_SPEED;				
	const float TIME_TO_WAIT_TILL_NEXT_SWITCH;	// sets how long to wait for the next fade in seconds
	sf::Clock switchClock;						// counts seconds till next crossfade
	float alpha1;								// alpha of 1st pic for crossfade
	float alpha2;								// alpha of 2nd pic for crossfade

	sf::RenderWindow* window;
	ResourceManager* resourceManager;
	SoundManager* soundManager;
	
	sf::RectangleShape background[10];			//splash //5 tut //back of menu
	sf::RectangleShape skipToMenuButton;


	bool goToMenu;								// control the loop
	bool leftMouseClicked;						// event
	sf::Vector2f mousePosition;					// contains mouseposition

};

