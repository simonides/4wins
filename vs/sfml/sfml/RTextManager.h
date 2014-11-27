#pragma once
#include <stdint.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include "GameState.h"

class ResourceManager;

namespace sf
{
	class RenderWindow;
}
	

class RTextManager
{
public:
	enum GameAction
	{
		SELECT_MEEPLE_POS, CHOOSE_A_MEEPLE
	};

	RTextManager(ResourceManager& resourceManager);
	~RTextManager();
	void update(float elapsedTime);
	void drawWinner(sf::RenderWindow& window, GameWinner::Enum winner) const;
	void drawTodo(sf::RenderWindow& window, GameAction action, uint8_t player);


private:

	float wobbleNR[18];
	float wobbleSin[18];

	float ANIMATIONSPEED;
	sf::RectangleShape white;
	sf::RectangleShape black;

	sf::RectangleShape choose;
	sf::RectangleShape select;
	sf::RectangleShape a;
	sf::RectangleShape meeple;
	sf::RectangleShape position;

	sf::RectangleShape tie;
	sf::RectangleShape wins;


};

