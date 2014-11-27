#include <SFML/Graphics.hpp>
#include "MenuConstants.h"

#include "Checkbox.h"

using namespace FourWins;

FourWins::Menu::Checkbox::Checkbox(sf::RenderWindow &window) :
	window(&window),
	box(new sf::RectangleShape()),
	boxCross(new sf::Text()),
	label(new sf::Text()),
	isVisible(true)
{}

FourWins::Menu::Checkbox::~Checkbox()
{
	delete this->box;
	delete this->boxCross;
	delete this->label;
}

void FourWins::Menu::Checkbox::init()
{
	this->box->setSize(sf::Vector2f(30.0f, 30.0f));
	this->box->setFillColor(Menu::BACKGROUND_COLOR);

	this->boxCross->setColor(Menu::TEXT_COLOR);

	this->label->setColor(Menu::LABEL_COLOR);
}

void FourWins::Menu::Checkbox::setFont(const sf::Font &font)
{
	this->boxCross->setFont(font);
	this->label->setFont(font);
}

void FourWins::Menu::Checkbox::setCharacterSize(unsigned int size)
{
	this->boxCross->setCharacterSize(size);
	this->label->setCharacterSize(size);
}

void FourWins::Menu::Checkbox::setPosition(const sf::Vector2f &position)
{
	this->box->setPosition(position);
	this->boxCross->setPosition(sf::Vector2f(position.x + 7.0f, position.y));
	this->label->setPosition(sf::Vector2f(position.x + 35.0f, position.y));
}

void FourWins::Menu::Checkbox::setLabelText(const sf::String &text)
{
	this->label->setString(text);
}

bool FourWins::Menu::Checkbox::isChecked() const
{
	return !this->boxCross->getString().isEmpty();
}

void FourWins::Menu::Checkbox::update(const sf::Event &e, const sf::Vector2f &mousePosition)
{
	this->box->setFillColor(Menu::BACKGROUND_COLOR);

	if (this->box->getGlobalBounds().contains(mousePosition))
	{
		switch (e.type)
		{
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseMoved:
			this->box->setFillColor(Menu::HIGHLIGHT_COLOR);
			break;
		case sf::Event::MouseButtonReleased:
			if (isChecked())
			{
				this->boxCross->setString("");
			}
			else
			{
				this->boxCross->setString("X");
			}
			this->box->setFillColor(Menu::HIGHLIGHT_COLOR);
			break;
		}
	}
}

void FourWins::Menu::Checkbox::draw()
{
	if (this->isVisible)
	{
		this->window->draw(*this->box);
		this->window->draw(*this->boxCross);
		this->window->draw(*this->label);
	}
}