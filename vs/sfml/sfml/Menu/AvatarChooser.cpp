#include <SFML/Graphics.hpp>

#include "AvatarChooser.h"

using namespace FourWins;

const float FourWins::Menu::AvatarChooser::THUMB_WIDTH = 50.0f;
const float FourWins::Menu::AvatarChooser::THUMB_HEIGHT = 75.0f;

FourWins::Menu::AvatarChooser::AvatarChooser(sf::RenderWindow &window, unsigned int choiceCount) :
	window(&window),
	bb(new sf::FloatRect()),
	choices(new AvatarChoice[choiceCount]),
	CHOICE_COUNT(choiceCount)
{
	this->bb->width = static_cast<float>(choiceCount / 2u) * AvatarChooser::THUMB_WIDTH;
	this->bb->height = 2.0f * AvatarChooser::THUMB_HEIGHT;

	for (unsigned int i = 0; i < this->CHOICE_COUNT; ++i)
	{
		this->choices[i].thumbnailShape = new sf::RectangleShape(sf::Vector2f(AvatarChooser::THUMB_WIDTH, AvatarChooser::THUMB_HEIGHT));
		this->choices[i].previewShape = new sf::RectangleShape();
		this->choices[i].previewText = new sf::Text();
		this->choices[i].previewTextShape = new sf::RectangleShape();
		this->choices[i].showPreview = false;
		this->choices[i].isSelected = false;
	}
}

FourWins::Menu::AvatarChooser::~AvatarChooser()
{
	for (unsigned int i = 0; i < this->CHOICE_COUNT; ++i)
	{
		delete this->choices[i].thumbnailShape;
		delete this->choices[i].previewShape;
		delete this->choices[i].previewText;
		delete this->choices[i].previewTextShape;
	}
	delete[] this->choices;
	delete this->bb;
}

void FourWins::Menu::AvatarChooser::init()
{
	
}

void FourWins::Menu::AvatarChooser::setPosition(const sf::Vector2f position)
{
	this->bb->left = position.x;
	this->bb->top = position.y;

	//calc thumbnailShapes
}

Menu::Avatars FourWins::Menu::AvatarChooser::getSelectedAvatar() const
{
	for (unsigned int i = 0; i < this->CHOICE_COUNT; ++i)
	{
		if (this->choices[i].isSelected)
		{
			return this->choices[i].avatar;
		}
	}
}

void FourWins::Menu::AvatarChooser::update(const sf::Event &e, const sf::Vector2f &mousePosition)
{
	resetHover();

	if (this->bb->contains(mousePosition))
	{
		switch (e.type)
		{
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseMoved:
			checkForHover(mousePosition);
			break;
		case sf::Event::MouseButtonReleased:
			checkForSelected(mousePosition);
			//resetHover();
			break;
		}
	}
}

void FourWins::Menu::AvatarChooser::draw()
{
	for (unsigned int i = 0; i < this->CHOICE_COUNT; ++i)
	{
		this->window->draw(*this->choices[i].thumbnailShape);

		if (this->choices[i].showPreview)
		{
			this->window->draw(*this->choices[i].previewShape);
			this->window->draw(*this->choices[i].previewTextShape);
			this->window->draw(*this->choices[i].previewText);
		}
	}
}

void FourWins::Menu::AvatarChooser::resetHover()
{
	for (unsigned int i = 0; i < this->CHOICE_COUNT; ++i)
	{
		this->choices[i].showPreview = this->choices[i].isSelected;
	}
}

void FourWins::Menu::AvatarChooser::checkForHover(const sf::Vector2f &mousePosition)
{
	for (unsigned int i = 0; i < this->CHOICE_COUNT; ++i)
	{
		this->choices[i].showPreview = this->choices[i].previewShape->getGlobalBounds().contains(mousePosition);
	}
}

void FourWins::Menu::AvatarChooser::checkForSelected(const sf::Vector2f &mousePosition)
{
	for (unsigned int i = 0; i < this->CHOICE_COUNT; ++i)
	{
		this->choices[i].isSelected = this->choices[i].previewShape->getGlobalBounds().contains(mousePosition);
	}
}