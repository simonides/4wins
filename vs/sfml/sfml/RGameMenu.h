#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "GameState.h"

#include "Game.h"



namespace sf{
	class RenderWindow;
}

class ResourceManager;
class SoundManager;
class RTextManager;

class RGameMenu
{
public:
	RGameMenu(ResourceManager& resourceManager, SoundManager& soundManager, RTextManager& textManager );
	~RGameMenu();
	void reset();
	void draw(sf::RenderWindow& render_window);
	void setMenuState(GameWinner::Enum winner);
	void resetHover();
	GameMenuDecision::Enum handleClickAndHover(InputEvents* inputEvents);

private:
	RGameMenu& operator=(const RGameMenu& rhs);
	const sf::Vector2f GAME_MENU_BUTTON_SIZE;
	const sf::Color GAME_MENU_BUTTON_COLOR;
	const sf::Color GAME_MENU_BUTTON_HOVER_COLOR;
	sf::RectangleShape* hoveredButtonPtr;
	sf::RectangleShape exitButton;
	sf::RectangleShape restartButton;
	sf::RectangleShape menuButton;
	GameWinner::Enum winner;

	RTextManager* textManager;
	SoundManager* soundManager;
	
};

