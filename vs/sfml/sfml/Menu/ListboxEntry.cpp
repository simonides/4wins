#include <SFML/Graphics.hpp>

#include "ListboxEntry.h"

using namespace FourWins;

FourWins::Menu::ListboxEntry::ListboxEntry() :
	background(new sf::RectangleShape()),
	text(new sf::Text()),
	isActive(false),
	value(0)
{}

FourWins::Menu::ListboxEntry::~ListboxEntry()
{
	delete this->background;
	delete this->text;
}

void FourWins::Menu::ListboxEntry::setPosition(const sf::Vector2f &position)
{
	this->background->setPosition(position);
	this->text->setPosition(sf::Vector2f(position.x + 5.0f, position.y));
}