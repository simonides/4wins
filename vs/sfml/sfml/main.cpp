
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
#include "ResourceManager.h"

#include "Menu/Menu.h"
#include "SoundManager.h"

const std::string GAME_TITLE = "4 Wins by ...";

sf::RenderWindow* setupWindow(){
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH_TO_CALCULATE, WINDOW_HEIGHT_TO_CALCULATE), GAME_TITLE);
	window->setPosition(sf::Vector2i(0, 0));
	//window->setVerticalSyncEnabled(true); //entweder das oder set frameratelimit
	window->setFramerateLimit(60);
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
	ResourceManager resourceManager;
    SoundManager soundManager;
	sf::RenderWindow* window = setupWindow();

	Menu::MainMenu* menu = new Menu::MainMenu(*window);
	menu->init();
	
	I_Player* player1 = new StupidAI();
    I_Player* player2 = new SmartAI(true, true);
    ThreadController* tc1 = new ThreadController(*player1);
    ThreadController* tc2 = new ThreadController(*player2);
    
    Player* players[2];
    players[0] = new Player();
    players[0]->type = Player::HUMAN;
    players[0]->player = nullptr;
    players[0]->controller = tc1;
	players[0]->playerAvatar = ResourceManager::ELLE;
    players[0]->meeplePositionThinkTime = { 0, 0.4 };
    players[0]->meepleChoosingThinkTime = { 0, 0.4 };

    players[1] = new Player();
    players[1]->type = Player::HUMAN;
    players[1]->player = nullptr;
    players[1]->controller = tc2;
    players[1]->playerAvatar = ResourceManager::RONALD;
    players[1]->meeplePositionThinkTime = { 1, 3 };
    players[1]->meepleChoosingThinkTime = { 1, 2 };

    Game* game = new Game(*window, players, resourceManager, soundManager);

    GameMenuDecision::Enum gameMenuDecision = GameMenuDecision::BACK_TO_MENU;

    while (window->isOpen()){
        if (gameMenuDecision == GameMenuDecision::BACK_TO_MENU){
            //menu->loop();
        }
        		
		gameMenuDecision = game->runGame();
        if (gameMenuDecision == GameMenuDecision::EXIT_GAME){
            break;
		}
	}
    delete game;
    delete players[0];
    delete players[1];
    delete tc2;
    delete tc1;
    delete player2;
    delete player1;
	delete menu;
	delete window;

	return 0;
}
