#pragma once

#include <SFML/Graphics.hpp>
#include "Game.h"

class ResourceManager;
struct Player;

class RBackground
{
private:
	sf::RectangleShape background;
	sf::RectangleShape leftWindow;
	sf::RectangleShape rightWindow;
	sf::RectangleShape avatarPlayer1;
	sf::RectangleShape avatarPlayer2;

	bool closeLeftWindow;
	bool closeRightWindow;

public:
	RBackground(ResourceManager& resourceLoader,Player& p1, Player& p2);
	~RBackground();
	
	void draw(sf::RenderWindow& window) const;

	void setLeftWindow(bool closeWindow);
	void setRightWindow(bool closeWindow);

	void update(float elapsedTime);

};

