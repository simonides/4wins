
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

#include "Game.h"
#include "GameSimulator.h"
#include "config.h"
#include "RandomAI.h"
#include "StupidAI.h"
#include "Player.h"
#include "ThinkingAI.h"
#include "IntelAI.h"

#include "Menu.h"

//using namespace std;


#define GAME_TITLE "4 Wins"




//+++++++++++++++++++++
#define MEEPLE_HIGHT 90
#define MEEPLE_WIDTH 45

#define MEEPLE_SNAP_X 22
#define MEEPLE_SNAP_Y 65

#define MEEPLE_SNAP_OFFSET_X 12
#define MEEPLE_SNAP_OFFSET_Y 30
//membervars!!!!
//+++++++++++++++++++++


sf::RenderWindow* setupWindow();


void AI_testFunction(){     
    I_Player* p1 = new ThinkingAI(true, true); //ThinkingAI(true, true);
    I_Player* p2 = new IntelAI(true, true); //RandomAI();// StupidAI();   //Player(
    GameSimulator* game = new GameSimulator(*p1, *p2);

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
		
		GameSettings gamesettings = menu->loop();
		//system("pause");
		
		Game* game = new Game(*window, *gamesettings.playerOne,*gamesettings.playerTwo);
		game->runGame();

		delete game;

	}

	delete menu;
	delete window;

	return 0;
}

sf::RenderWindow* setupWindow(){
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Application");
	window->setPosition(sf::Vector2i(0, 0));
	window->setVerticalSyncEnabled(true); //entweder das oder set frameratelimit

	return window;
}

//srand(static_cast<unsigned int>(time(NULL)));

//AI_testFunction();

//--------------------
//membervars
/*	bool dragme = false;
sf::Vector2f dragmeVect(0, 0);
sf::Vector2f oldPos(30, 30);
*///--------------------



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
//bool keypressedK = false;
//bool glow = false;

//	//window.setFramerateLimit(30);
//sf::RectangleShape leftPanel(sf::Vector2f(200.f,690.f));
//leftPanel.setPosition(sf::Vector2f(0.f, 0.f));
//leftPanel.setFillColor(sf::Color::Green);

//sf::RectangleShape rightPanel(sf::Vector2f(200.f,690.f));
//rightPanel.setPosition(sf::Vector2f(1150.f, 0.f));
//rightPanel.setFillColor(sf::Color::Magenta);


//sf::RectangleShape boardPanel(sf::Vector2f(450.f, 450.f));
//boardPanel.setFillColor(sf::Color::White);
//boardPanel.setPosition(sf::Vector2f(BOARD_X_OFFSET, BOARD_Y_OFFSET- 95.f));
//boardPanel.setRotation(45.f);
//boardPanel.setRotation(45.f);


//sf::CircleShape* squares = new sf::CircleShape[16]();

//
//int counter = 0;
//for (int y= 0; y < 7; ++y){

//	//cout << "counter in loop: " << counter << endl;
//	

//	int times = (y + 1) > 3 ? 7 - y  : y + 1;
//	cout << "times: "  << times << endl;
//	
//	float calcX = BOARD_X_OFFSET - ((times -1)/2.f *BOARD_X_SPACEING) ;

//	for (int x = 0; x < times; ++x){	
//		squares[counter].setOrigin(sf::Vector2f(40.f, 40.f));
//		squares[counter].setFillColor(sf::Color::Yellow);
//		squares[counter].setRadius(40);
//		squares[counter].setPointCount(6);
//		squares[counter].setPosition(sf::Vector2f(calcX, y * BOARD_Y_SPACEING + BOARD_Y_OFFSET));
//		calcX += BOARD_X_SPACEING;
//		++counter;
//	}
//}
//
//
//sf::RectangleShape meeple(sf::Vector2f(MEEPLE_WIDTH,MEEPLE_HIGHT));
//
//sf::Texture texture;
//// load a 32x32 rectangle that starts at (10, 10)
//if (!texture.loadFromFile(WORKING_DIR+"field.jpg"))
//{
//	//error
//
//}
//texture.setRepeated(true);
////textur
//cout << "load tex..." << endl;
//meeple.setTexture(&texture);
//meeple.setTextureRect(sf::IntRect(10, 10, 32, 32));

////meeple.setTexture(&texture);

////meeple.setFillColor(sf::Color::Blue);
//meeple.setPosition(30,30);

//while (window->isOpen()){
//
//	sf::Event event;
//	while (window->pollEvent(event)){

//		if (event.type == sf::Event::Closed){
//			window->close();
//		}

//		if (event.type == sf::Event::KeyPressed)
//		{
//			if (event.key.code == sf::Keyboard::K)
//			{
//				cout << "key k pressed " << endl;
//				keypressedK = true;
//			}
//		}
//		if (event.type == sf::Event::KeyReleased)
//		{
//			if (event.key.code == sf::Keyboard::K)
//			{
//				cout << "key k released " << endl;
//				keypressedK = false;
//			}
//		}
//		if (event.type == sf::Event::MouseWheelMoved)
//		{
//			//std::cout << "wheel movement: " << event.mouseWheel.delta << std::endl;
//			//std::cout << "mouse x: " << event.mouseWheel.x << std::endl;
//			//std::cout << "mouse y: " << event.mouseWheel.y << std::endl;
//		}

//		if (event.type == sf::Event::MouseButtonReleased){
//			if (event.mouseButton.button == sf::Mouse::Left){
//				dragme = false;
//				sf::Vector2f meeplePos(meeple.getGlobalBounds().left + MEEPLE_SNAP_X, meeple.getGlobalBounds().top +MEEPLE_SNAP_Y);
//				for (int i = 0; i < 16; ++i){

//					if (squares[i].getGlobalBounds().contains(meeplePos)){
//						cout << "snap to: " << i << endl;
//						
//						//squares[i].getGlobalBounds().left
//						//oldPos= dragmeVect;
//						oldPos = sf::Vector2f(meeple.getGlobalBounds().left, meeple.getGlobalBounds().top);
//							//oldPos = sf::Vector2f(squares[i].getGlobalBounds().left+ MEEPLE_SNAP_OFFSET_X, squares[i].getGlobalBounds().top- MEEPLE_SNAP_OFFSET_Y);
//					
//					}
//					

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
//			if (event.mouseButton.button == sf::Mouse::Right)
//			{
//				
//				//std::cout << "the right button was pressed" << std::endl;
//				//std::cout << "mouse x: " << event.mouseButton.x << std::endl;
//				//std::cout << "mouse y: " << event.mouseButton.y << std::endl ;
//			}
//		}
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

//		}
//	}

//window->clear(sf::Color::Black);
//window->draw(boardPanel);


//for (int i = 0; i < 16; ++i){
//	window->draw(squares[i]);
//	//squares[i].setPointCount(5);
//}

//window->draw(rightPanel);
//window->draw(leftPanel);
//window->draw(meeple);
//window->display();


//delete window;
//return 0;

//}
