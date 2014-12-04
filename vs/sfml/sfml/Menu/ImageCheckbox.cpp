#include <SFML/Graphics.hpp>

#include "ImageCheckbox.h"

using namespace FourWins;

FourWins::Menu::ImageCheckbox::ImageCheckbox(sf::RenderWindow &window) :
	window(&window),
	shape(new sf::RectangleShape()),
	textureRect(new sf::IntRect()),
	textureCheckedRect(new sf::IntRect()),
	isChecked(false)
{}

FourWins::Menu::ImageCheckbox::~ImageCheckbox()
{
	delete this->shape;
	delete this->textureRect;
	delete this->textureCheckedRect;
}

void FourWins::Menu::ImageCheckbox::setTextureRect(const sf::IntRect &rect)
{
	this->textureRect->left = rect.left;
	this->textureRect->top = rect.top;
	this->textureRect->width = rect.width;
	this->textureRect->height = rect.height;
	this->shape->setTextureRect(rect);
}

void FourWins::Menu::ImageCheckbox::setTextureHighlightRect(const sf::IntRect &rect)
{
	this->textureCheckedRect->left = rect.left;
	this->textureCheckedRect->top = rect.top;
	this->textureCheckedRect->width = rect.width;
	this->textureCheckedRect->height = rect.height;
}

void FourWins::Menu::ImageCheckbox::update(const sf::Event &e, const sf::Vector2f &mousePosition)
{
	if (this->shape->getGlobalBounds().contains(mousePosition) &&
		e.type == sf::Event::MouseButtonReleased)
	{
		this->isChecked = !this->isChecked;
		switchTextureRect();
	}
}

void FourWins::Menu::ImageCheckbox::draw()
{
	this->window->draw(*this->shape);
}

void FourWins::Menu::ImageCheckbox::switchTextureRect()
{
	if (this->isChecked)
	{
		this->shape->setTextureRect(*this->textureCheckedRect);
	}
	else
	{
		this->shape->setTextureRect(*this->textureRect);
	}
}