#include "Menu.h"

#include <SFML/Graphics.hpp>

#include "ThinkingAI.h"


#define BOARD_X_OFFSET 675
#define BOARD_Y_OFFSET 150

Menu::Menu(sf::RenderWindow* window) : window(window), startgame(false)
{ 
	

}




Menu::~Menu()
{
}

GameSettings Menu::loop(){
	
	sf::Font font;
	if (!font.loadFromFile(WORKING_DIR + "Fonts/roboto/Roboto-Thin.ttf")){ 	
		std::cout << "error" << std::endl;
		exit(-1);
		/*return;*/ 
	}

	sf::Text header;
	header.setFont(font);

	header.setString("Menu");
	header.setCharacterSize(120); // in pixels, not points!
	header.setColor(sf::Color::Blue);
	header.setStyle(sf::Text::Bold);
	header.setPosition(sf::Vector2f(500, 145));

	button = sf::RectangleShape(sf::Vector2f(278,50));
	button.setFillColor(sf::Color::Blue);
	button.setPosition(sf::Vector2f(512, 320));

	sf::Text buttontext;
	buttontext.setFont(font);
	buttontext.setString("Start new game");
	buttontext.setCharacterSize(36);
	buttontext.setColor(sf::Color::Yellow);
	buttontext.setStyle(sf::Text::Bold);
	buttontext.setPosition(sf::Vector2f(520, 321));
	

	while (window->isOpen() && startgame == false){

		pollEvents();

		window->clear(sf::Color::Black);

		window->draw(header);
		window->draw(button);
		window->draw(buttontext);

		window->display();
	}


	I_Player* thinking = new ThinkingAI(true, true);
	return GameSettings(thinking, thinking);
}

void Menu::pollEvents(){
	sf::Event event;
	while (window->pollEvent(event)){
		sf::Vector2i mousepos = sf::Mouse::getPosition(*window);
		sf::Vector2f converted = window->mapPixelToCoords(mousepos);

		if (event.type == sf::Event::Closed){ window->close(); }

		//if (event.type == sf::Event::KeyPressed){
			//if (event.key.code == sf::Keyboard::K){	}
		//}
		//if (event.type == sf::Event::KeyReleased){
			//if (event.key.code == sf::Keyboard::K){	}
		//}
		//if (event.type == sf::Event::MouseWheelMoved){	}

		//if (event.type == sf::Event::MouseButtonPressed)
		//{
			//if (event.mouseButton.button == sf::Mouse::Left){
				//std::cout << "mouse x: " << event.mouseButton.x << std::endl;
				//std::cout << "mouse y: " << event.mouseButton.y << std::endl;
			//}
			//if (event.mouseButton.button == sf::Mouse::Right){
				//std::cout << "the right button was pressed" << std::endl;
			//}
		//}
		if (event.type == sf::Event::MouseButtonReleased){
			if (event.mouseButton.button == sf::Mouse::Left){
				if (button.getGlobalBounds().contains(converted)){
					std::cout << "start game " << std::endl;
					startgame = true;
				}
			}
		}
		if (event.type == sf::Event::MouseMoved){
				
			if (button.getGlobalBounds().contains(converted)){
				button.setFillColor(sf::Color(10,100,170,255));
			}
			else{
				button.setFillColor(sf::Color::Blue);
			}
		}
	}
}