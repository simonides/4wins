#include <SFML/Graphics.hpp>

#include "boardState.h"

#include "MeepleBag.h"


int main(){
    
    delete new MeepleBag(WHITE);



	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Application");
	sf::CircleShape shape;
	shape.setRadius(40.f);
	shape.setPosition(100.f, 100.f);
	shape.setFillColor(sf::Color::Cyan);
	//jakob ist der meister des c++
	while (window.isOpen()){
        //und sebastian ist neidisch darauf    
		sf::Event event;
		while (window.pollEvent(event)){

			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

}