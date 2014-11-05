
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

#include "Game.h"
#include "config.h"
#include "RandomAI.h"
#include "StupidAI.h"
#include "Player.h"
#include "ThinkingAI.h"

using namespace std;

int main(){
    srand(static_cast<unsigned int>(time(NULL)));






    /*
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(WORKING_DIR + "test.mp3"))
        return -1;


    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();*/

    sf::Music music;
    if (!music.openFromFile(WORKING_DIR + "test.ogg"))
        return -1; // error
    music.play();
    music.setLoop(true);



    unsigned int pw1 = 0, pw2 = 0, ties=0;
   
    I_Player* p1 = new ThinkingAI(true, false);
    I_Player* p2 = new ThinkingAI(false, true); //RandomAI();// StupidAI();   //Player(/*inject sfml-objects for user input here*/);
    Game* game = new Game(*p1, *p2);
    
    std::cout << "Calculating..."<<std::endl;
    for (int g = 0;g<10000;g++){
        
        GameWinner::Enum winner = game->runGame();      //DIESE METHODE darf umgeschrieben werden, damit es mit sfml kompatibel wird
        switch (winner){
            case GameWinner::PLAYER_1: pw1++; break;
            case GameWinner::PLAYER_2: pw2++; break;
            case GameWinner::TIE: ties++;  break;
        }

        game->reset();
        
    }
    delete p1;
    delete p2;
    delete game;



    std::cout << "Player 1 won " << pw1 << " times, and Player 2 won " << pw2 << " times. There were " << ties << " Ties."<<std::endl;

    music.stop();

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