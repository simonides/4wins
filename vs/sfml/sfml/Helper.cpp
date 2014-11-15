#include "helper.h"
#include <SFML/Graphics.hpp>


void handleResizeWindowEvent(sf::RenderWindow* window){
	if (window->getSize().y == 0){ return; }
	float ratio = static_cast<float>(window->getSize().x) / static_cast<float>(window->getSize().y);
	if (!(ratio >= 1.75f && ratio <= 1.8)){
		window->setSize(sf::Vector2u(window->getSize().x, static_cast<unsigned int>(static_cast<float>(window->getSize().x) / 16.f * 9.f)));
	}
}
