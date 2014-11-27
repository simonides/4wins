#include <SFML/Graphics.hpp>
#include "MenuConstants.h"

#include "Button.h"

using namespace FourWins;

FourWins::Menu::Button::Button(sf::RenderWindow &window) :
	window(&window),
	background(new sf::RectangleShape()),
	textureRect(new sf::IntRect()),
	textureHighlightRect(new sf::IntRect()),
	isReleased(false)
{}

FourWins::Menu::Button::~Button()
{
	delete this->background;
	delete this->textureRect;
	delete this->textureHighlightRect;
}

void FourWins::Menu::Button::init()
{
	this->background->setSize(sf::Vector2f(350.0f, 60.0f));
	this->background->setPosition(sf::Vector2f(500.0f, 550.0f));
	this->background->setTextureRect(*this->textureRect);
}


void FourWins::Menu::Button::setPosition(const sf::Vector2f &position)
{
	this->background->setPosition(position);
}

void FourWins::Menu::Button::setSize(const sf::Vector2f &size)
{
	this->background->setSize(size);
}

void FourWins::Menu::Button::setTexture(const sf::Texture *texture)
{
	this->background->setTexture(texture);
}

void FourWins::Menu::Button::setTextureRect(const sf::IntRect &rect)
{
	this->textureRect->left = rect.left;
	this->textureRect->top = rect.top;
	this->textureRect->width = rect.width;
	this->textureRect->height = rect.height;
}

void FourWins::Menu::Button::setTextureHighlightRect(const sf::IntRect &rect)
{
	this->textureHighlightRect->left = rect.left;
	this->textureHighlightRect->top = rect.top;
	this->textureHighlightRect->width = rect.width;
	this->textureHighlightRect->height = rect.height;
}

bool FourWins::Menu::Button::getIsReleased() const
{
	return this->isReleased;
}

void FourWins::Menu::Button::update(const sf::Event &e, const sf::Vector2f &mousePosition)
{
	this->isReleased = false;
	this->background->setTextureRect(*this->textureRect);

	if (this->background->getGlobalBounds().contains(mousePosition))
	{
		switch (e.type)
		{
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseMoved:
			this->background->setTextureRect(*this->textureHighlightRect);
			break;
		case sf::Event::MouseButtonReleased:
			this->isReleased = true;
			break;
		}
	}
}

void FourWins::Menu::Button::draw()
{
	this->window->draw(*this->background);
}