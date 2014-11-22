
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <assert.h>

#include "Game.h"
#include "GameSimulator.h"
#include "ThreadedGameSimulator.h"
#include "config.h"
#include "RandomAI.h"
#include "StupidAI.h"
#include "ThinkingAI.h"
#include "SmartAI.h"
#include "ResourceLoader.h"

#include "Menu/Menu.h"

const std::string GAME_TITLE = "4 Wins by ...";

sf::RenderWindow* setupWindow(){
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH_TO_CALCULATE, WINDOW_HEIGHT_TO_CALCULATE), GAME_TITLE);
	window->setPosition(sf::Vector2i(0, 0));
	//window->setVerticalSyncEnabled(true); //entweder das oder set frameratelimit
	//window->setFramerateLimit(2);
	window->setSize(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT));// 16:9
	return window;
}

void AI_testFunction(){     
    I_Player* p1 = new SmartAI();// (true, true); //ThinkingAI(true, true);
    I_Player* p2 = new ThinkingAI();// (true, true); //RandomAI();// StupidAI();   //Player(
    GameSimulator* game = new ThreadedGameSimulator(*p1, *p2);//GameSimulator(*p1, *p2);

    GameWinner::Enum winner = game->runManyGames(15000, true);
   
    delete game;
    delete p2;
    delete p1;

    std::cout << "And the winner is: " << (winner == GameWinner::TIE ? "NOONE - TIE!" : (winner == GameWinner::PLAYER_1 ? "Player 1" : "Player 2")) << std::endl;
    std::cin.ignore();   //wait for keypress
    exit(0);
}

using namespace FourWins;

int main(){
	//AI_testFunction();
	ResourceLoader resourceLoader;
	sf::RenderWindow* window = setupWindow();

	Menu::MainMenu* menu = new Menu::MainMenu(*window);
	menu->init();





	I_Player* test = new StupidAI();
	I_Player* test2 = new SmartAI(true, true);

	ThreadController* tc1 = new ThreadController(*test2);
	ThreadController* tc2 = new ThreadController(*test2);
	//I_Player* test2 =	new Player();

	Player* p1 = new Player();
	p1->type = Player::TC;
	p1->player = nullptr;
	p1->controller = tc1;

	Player* p2 = new Player();
	p2->type = Player::TC;
	p2->player = nullptr;
	p2->controller = tc2;

	Player* human = new Player();
	human->type = Player::HUMAN;
	human->player = nullptr;

	Player* human2 = new Player();
	human2->type = Player::HUMAN;
	human2->player = nullptr;

	assert(p1 != nullptr);
	assert(p2 != nullptr);
	assert(human != nullptr);
	assert(human2 != nullptr);
	assert(tc2 != nullptr);
	assert(tc1 != nullptr);

	while (window->isOpen()){

		menu->loop();

		Game* game = new Game(*window, *p2, *p1, resourceLoader);
		GameReturn returnValue = game->runGame();
		if (returnValue == EXIT)
		{
			delete game;
			delete p2;
			delete p1;
			delete test;
			delete test2;
			delete tc1;
			delete tc2;
			break;
		}
		else
		{
			delete game;
			continue;
		}




	}

	//delete menu;
	delete window;

	return 0;
}

/*
sf::Music music;
if (!music.openFromFile(WORKING_DIR + "test.ogg"))
return -1; // error
music.play();
music.setLoop(true);

cin.ignore();   //wait for keypress

music.stop();
return 0;
*/
