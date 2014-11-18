
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

#include "Game.h"
#include "GameSimulator.h"
#include "ThreadedGameSimulator.h"
#include "config.h"
#include "RandomAI.h"
#include "StupidAI.h"
#include "ThinkingAI.h"
#include "SmartAI.h"

#include "Menu.h"

const std::string GAME_TITLE = "4 Wins by ...";

sf::RenderWindow* setupWindow(){
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH_TO_CALCULATE, WINDOW_HEIGHT_TO_CALCULATE), GAME_TITLE);
	window->setPosition(sf::Vector2i(0, 0));
	window->setVerticalSyncEnabled(true); //entweder das oder set frameratelimit
	//window->setFramerateLimit(30);
	window->setSize(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT));// 16:9
	return window;
}

void AI_testFunction(){     
    I_Player* p1 = new SmartAI();// (true, true); //ThinkingAI(true, true);
    I_Player* p2 = new ThinkingAI();// (true, true); //RandomAI();// StupidAI();   //Player(
    GameSimulator* game = new ThreadedGameSimulator(*p1, *p2);//GameSimulator(*p1, *p2);

    GameWinner::Enum winner = game->runManyGames(10000, true);
   
    delete game;
    delete p2;
    delete p1;

    std::cout << "And the winner is: " << (winner == GameWinner::TIE ? "NOONE - TIE!" : (winner == GameWinner::PLAYER_1 ? "Player 1" : "Player 2")) << std::endl;
    std::cin.ignore();   //wait for keypress
    exit(0);
}


int main(){
	//AI_testFunction();
	
	sf::RenderWindow* window = setupWindow();
	Menu* menu = new Menu(window);
	while (window->isOpen()){
		
		//GameSettings gamesettings = menu->loop();
		//system("pause");
		
		I_Player* test =	new ThinkingAI(true, true);
		ThreadController* tc = new ThreadController(*test);
		//I_Player* test2 =	new Player();

		Player* p1 = new Player();
		p1->type = Player::HUMAN;
		p1->player = nullptr;

		Player* p2 = new Player();
		p2->type = Player::TC;
		p2->controller = tc;
		//p1.player

		//ThreadController* test3 = new ThreadController(*test2);



		Game* game = new Game(*window, *p1, *p2);
		game->runGame();

		delete game;
		delete p2;
		delete p1;
		break; // or start new game .. 

	}

	delete menu;
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


//--------------------
//membervars
/*	bool dragme = false;
sf::Vector2f dragmeVect(0, 0);
sf::Vector2f oldPos(30, 30);
*///--------------------

//		if (event.type == sf::Event::MouseButtonReleased){
//			if (event.mouseButton.button == sf::Mouse::Left){
//				dragme = false;
//				sf::Vector2f meeplePos(meeple.getGlobalBounds().left + MEEPLE_SNAP_X, meeple.getGlobalBounds().top +MEEPLE_SNAP_Y);
//				for (int i = 0; i < 16; ++i){
//
//					if (squares[i].getGlobalBounds().contains(meeplePos)){
//						cout << "snap to: " << i << endl;
//						
//						//squares[i].getGlobalBounds().left
//						//oldPos= dragmeVect;
//						oldPos = sf::Vector2f(meeple.getGlobalBounds().left, meeple.getGlobalBounds().top);
//							//oldPos = sf::Vector2f(squares[i].getGlobalBounds().left+ MEEPLE_SNAP_OFFSET_X, squares[i].getGlobalBounds().top- MEEPLE_SNAP_OFFSET_Y);
//					
//					}
//				}
//				//if ( meeple.getGlobalBounds())
//				meeple.setPosition(oldPos);
//			}
//		}

//		if (event.type == sf::Event::MouseButtonPressed)
//		{
//			if (event.mouseButton.button == sf::Mouse::Left)
//			{
//				sf::Vector2i mousepos = sf::Mouse::getPosition(*window);
//				sf::Vector2f converted = window->mapPixelToCoords(mousepos);

//				std::cout << "the left button was pressed" << std::endl;

//				if (meeple.getGlobalBounds().contains(converted)){
//					dragme = true;
//					cout << "globalbounds x|y : " << meeple.getGlobalBounds().left << "|" << meeple.getGlobalBounds().top << endl;
//					cout << "converted x|y : " << converted.x << "|" << converted.y << endl <<endl;
//					dragmeVect.x = converted.x - meeple.getGlobalBounds().left;
//					dragmeVect.y = converted.y - meeple.getGlobalBounds().top;

//					oldPos.x = meeple.getGlobalBounds().left;
//					oldPos.y = meeple.getGlobalBounds().top;
//					
//				}


//				//std::cout << "mouse x: " << event.mouseButton.x << std::endl;
//				//std::cout << "mouse y: " << event.mouseButton.y << std::endl;
//			}
//		if (event.type == sf::Event::MouseMoved)
//		{
//			sf::Vector2i mousepos = sf::Mouse::getPosition(*window);
//			sf::Vector2f converted = window->mapPixelToCoords(mousepos);

//			for (int i = 0; i < 16; ++i){
//				if (squares[i].getGlobalBounds().contains(converted)){
//					squares[i].setFillColor(sf::Color::Red);
//				}
//				else{
//					squares[i].setFillColor(sf::Color::Yellow);
//				}
//				//window.draw(squares[i]);
//				//squares[i].setPointCount(5);
//			}

//			if (meeple.getGlobalBounds().contains(converted)){
//				//meeple.setFillColor(sf::Color::Red);
//			}
//			else{
//				//meeple.setFillColor(sf::Color::Blue);
//			}

//			if (dragme){
//				meeple.setPosition(converted-dragmeVect);
//			}

//			//cout << "converted.x " << converted.x << "|| converted.y " << converted.y << endl;
//			//glow = square.getGlobalBounds().contains(converted);

//			//std::cout << "new mouse x: " << event.mouseMove.x << std::endl;
//			//std::cout << "new mouse y: " << event.mouseMove.y << std::endl << endl;

