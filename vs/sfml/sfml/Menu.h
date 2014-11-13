#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "config.h"
#include "I_Player.h"

struct GameSettings{
	I_Player* playerOne;
	I_Player* playerTwo;
	GameSettings(I_Player* playerOne, I_Player* playerTwo) : playerOne(playerOne), playerTwo(playerTwo){}
};



class Menu
{
private:
	sf::RenderWindow* window;
	sf::RectangleShape button;

	bool startgame;
	void pollEvents();

public:
	explicit Menu(sf::RenderWindow* window);
	virtual ~Menu();

	//ich würd hier ein struct mit den playerdaten/spieleinstellungen 
	// zurückgeben und der gameloop zum initialisieren geben.
	//per value zurückgeben????
	GameSettings loop();


};

