
#include <windows.h>
//#include <wincon.h> 
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
#include "PreMenu.h"

#define PI 3.14159265


sf::RenderWindow* setupWindow(){
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH_TO_CALCULATE, WINDOW_HEIGHT_TO_CALCULATE), WINDOW_TITLE);
	window->setPosition(sf::Vector2i(0, 0));
	//window->setVerticalSyncEnabled(true); //entweder das oder set frameratelimit -> laut doku besser das framelimit
	window->setFramerateLimit(60);
	window->setSize(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT));// 16:9
	
	sf::Image icon; // maybe illegal to keep it here but it works
	if (icon.loadFromFile(WORKING_DIR "icon.png")) {
		window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
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


void displaySplashScreen(sf::RenderWindow& window)
{
	sf::Texture splashscreen;
	if (!splashscreen.loadFromFile(WORKING_DIR "splashscreen.png")){
		std::cerr << "Couldn't load the texture: splashscreen" << std::endl;
		exit(1);
	}
	splashscreen.setSmooth(true);

	float width = static_cast<float>(WINDOW_WIDTH_TO_CALCULATE);
	float height = static_cast<float>(WINDOW_HEIGHT_TO_CALCULATE);

	sf::RectangleShape splashScreenRect;
	splashScreenRect.setPosition(0, 0);
	splashScreenRect.setSize(sf::Vector2f(width, height));
	splashScreenRect.setTexture(&splashscreen);
	splashScreenRect.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH_TO_CALCULATE, WINDOW_HEIGHT_TO_CALCULATE));
	splashScreenRect.setFillColor(sf::Color(255, 255, 255, 255));

	window.clear();
	window.draw(splashScreenRect);
	window.display();
}


using namespace FourWins;

int main(int argc, char *argv[]){  

	

	    
    GameSettings* settings = nullptr;
    Player* players[2] = {nullptr, nullptr};
    Game* game = nullptr;
    GameMenuDecision::Enum gameMenuDecision = GameMenuDecision::KEEP_PLAYING;
      
    //argc = 1;
    //argv[argc++] = "TEST";
    //argv[argc++] = "-sim=500";
    //argv[argc++] = "-p1=stupid";
    //argv[argc++] = "-p2=smart";

    if (argc > 1){          //Parse program parameters
        settings = parseConsoleParameters(argc, argv);
        if (settings == nullptr){
            print_usage(argv[0]);
			system("pause");
			exit(0);
        }
    }
    if (settings != nullptr && settings->simulator > 0){
        AI_testFunction(*settings);
        exit(0);
    }
	ShowWindow(GetConsoleWindow(), SW_HIDE); //hide console window .. hide only because it is needed for the network ais

	sf::RenderWindow* window = setupWindow();
	displaySplashScreen(*window);

	ResourceManager resourceManager;
	SoundManager soundManager;

	PreMenu* preMenu = new PreMenu(*window, resourceManager, soundManager);
   
	Menu::MainMenu* menu = new Menu::MainMenu(*window);
    menu->init(resourceManager, soundManager);
	
	preMenu->runLoop();
	
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
