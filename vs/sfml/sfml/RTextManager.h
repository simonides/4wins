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

	float wobbleNR;
	float wobbleSin[2];

	float chooseX;
	float arrowX;
	float selectX;
	float positionX;
	float meepleX;

	float ANIMATIONSPEED;
	sf::RectangleShape player1;
	sf::RectangleShape player2;

	sf::RectangleShape choose;
	sf::RectangleShape select;
	sf::RectangleShape arrowUp;
	sf::RectangleShape meeple;
	sf::RectangleShape position;

	sf::RectangleShape tie;
	sf::RectangleShape wins;


};

