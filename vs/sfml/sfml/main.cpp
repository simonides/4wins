
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <assert.h>
#include <math.h>

#include "Player.h"
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
#include "GameSettings.h"
#include "getopt.h"

#define PI 3.14159265

const std::string GAME_TITLE = "4 Wins by ...";


sf::RenderWindow* setupWindow(){
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH_TO_CALCULATE, WINDOW_HEIGHT_TO_CALCULATE), GAME_TITLE);
	window->setPosition(sf::Vector2i(0, 0));
	//window->setVerticalSyncEnabled(true); //entweder das oder set frameratelimit
	window->setFramerateLimit(60);
	window->setSize(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT));// 16:9
	return window;
}


void AI_testFunction(const GameSettings& settings){
    I_Player* p1 = createI_Player(settings, 0);
    I_Player* p2  = createI_Player(settings, 1);
    GameSimulator* game;
    
    if (settings.threadedSimulator){
        game = new ThreadedGameSimulator(*p1, *p2);     //Takes ownership of both players
    }else{
        game = new GameSimulator(*p1, *p2);
    }   
    GameWinner::Enum winner = game->runManyGames(settings.simulator, true);   
    delete game;
    if (!settings.threadedSimulator){
        delete p2;
        delete p1;
    }
    std::cout << "And the winner is: " << (winner == GameWinner::TIE ? "NOONE - TIE!" : (winner == GameWinner::PLAYER_1 ? "Player 1" : "Player 2")) << std::endl;
    std::cin.ignore();   //wait for keypress
}





using namespace FourWins;

int main(int argc, char *argv[]){  
	ResourceManager resourceManager;
    SoundManager soundManager;
	    
    GameSettings* settings = nullptr;
    Player* players[2] = {nullptr, nullptr};
    Game* game = nullptr;
    GameMenuDecision::Enum gameMenuDecision = GameMenuDecision::KEEP_PLAYING;
      
    //argc = 0;
    //argv[argc++] = "TEST";
    //argv[argc++] = "-sim=500";
    //argv[argc++] = "-p1=stupid";
    //argv[argc++] = "-p2=smart";

    if (argc > 1){          //Parse program parameters
        settings = parseConsoleParameters(argc, argv);
        if (settings == nullptr){
            print_usage(argv[0]);
        }
    }
    if (settings != nullptr && settings->simulator > 0){
        AI_testFunction(*settings);
        exit(0);
    }


    sf::RenderWindow* window = setupWindow();

    Menu::MainMenu* menu = new Menu::MainMenu(*window);
    menu->init(resourceManager);

        
    while (window->isOpen()){
        if (settings == nullptr || gameMenuDecision == GameMenuDecision::BACK_TO_MENU){            
            delete settings;        //Delete the previous settings
            delete game;
            game = nullptr;
			settings = menu->loop();
        }
        if (game == nullptr){            
            delete players[0];
            delete players[1];
            players[0] = createPlayer(*settings, 0);
            players[1] = createPlayer(*settings, 1);
            game = new Game(*window, players, resourceManager, soundManager);
        }
                
        if (!window->isOpen()){
            break;
        }    

		gameMenuDecision = game->runGame();
        if (gameMenuDecision == GameMenuDecision::EXIT_GAME){
            break;
		}
	}

    delete game;
    delete settings;
    delete players[0];
    delete players[1];
	delete menu;
	delete window;

	return 0;
}
