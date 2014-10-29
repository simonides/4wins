#include "game.h"
#include <SFML/Graphics.hpp>


int main(){

	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Application");
	
	
	
	sf::CircleShape shape;
	shape.setRadius(40.f);
	shape.setPosition(100.f, 100.f);
	shape.setFillColor(sf::Color::Cyan);
	//jakob ist der meister des c++
	while (window.isOpen()){
        //und sebastian ist neidisch darauf    
		
		// Load a sprite to display
		//sf::Texture texture;
		//if (!texture.loadFromFile(WORKING_DIR+"3996.jpg" ))
			//return EXIT_FAILURE; 
		//sf::Sprite sprite(texture);
		
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
		//window.draw(sprite);
		window.draw(text);

		window.display();
	}

}