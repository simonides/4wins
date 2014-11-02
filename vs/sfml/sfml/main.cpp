
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "Game.h"
#include "config.h"
#include "RandomAI.h"
#include "StupidAI.h"


using namespace std;

int main(){
    
    I_Player* p1 = new RandomAI();
    I_Player* p2 = new RandomAI();// StupidAI();

    Game* game = new Game(*p1, *p2);
    GameWinner::Enum winner = game->runGame();
    switch (winner){
        case GameWinner::PLAYER_1: cout << "Player 1 wins!" << endl; break;
        case GameWinner::PLAYER_2: cout << "Player 2 wins!" << endl; break;
        case GameWinner::TIE: cout << "Tie! There is no winner." << endl; break;
    }
    
    delete p1;
    delete p2;
    delete game;

    cin.ignore();   //wait for keypress
    return 0;






	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Application");
	
	
	
	sf::CircleShape shape;
	shape.setRadius(40.f);
	shape.setPosition(100.f, 100.f);
	shape.setFillColor(sf::Color::Cyan);
	//jakob ist der meister des c++
	while (window.isOpen()){
        //und sebastian ist neidisch darauf    
		
		// Load a sprite to display
		sf::Texture texture;
		if (!texture.loadFromFile(WORKING_DIR+"3996.jpg" ))
		return EXIT_FAILURE; 
		sf::Sprite sprite(texture);
		
		// Create a graphical text to display
		sf::Font font;
		if (!font.loadFromFile(WORKING_DIR+"/Fonts/roboto/Roboto-Light.ttf"))
			return EXIT_FAILURE;
		sf::Text text("Hello SFML", font, 50);
		
		
		sf::Event event;
		while (window.pollEvent(event)){

			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.draw(sprite);
		window.draw(text);

		window.display();
	}

}