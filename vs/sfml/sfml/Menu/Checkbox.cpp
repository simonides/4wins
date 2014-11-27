#include <SFML/Graphics.hpp>
#include "MenuConstants.h"

#include "Checkbox.h"

using namespace FourWins;

FourWins::Menu::Checkbox::Checkbox(sf::RenderWindow &window) :
	window(&window),
	boxes(new sf::RectangleShape[2]),
	boxCrosses(new sf::Text[2]),
	label(new sf::Text()),
	isVisible(new bool[2])
{
	this->isVisible[0] = false;
	this->isVisible[1] = false;
}

FourWins::Menu::Checkbox::~Checkbox()
{
	delete[] this->boxes;
	delete[] this->boxCrosses;
	delete this->label;
	delete[] this->isVisible;
}

void FourWins::Menu::Checkbox::init()
{
	this->boxes[0].setSize(sf::Vector2f(30.0f, 30.0f));
	this->boxes[0].setFillColor(Menu::BACKGROUND_COLOR);

	this->boxes[1].setSize(sf::Vector2f(30.0f, 30.0f));
	this->boxes[1].setFillColor(Menu::BACKGROUND_COLOR);

	this->boxCrosses[0].setColor(Menu::TEXT_COLOR);
	this->boxCrosses[1].setColor(Menu::TEXT_COLOR);

	this->label->setColor(Menu::LABEL_COLOR);
}

void FourWins::Menu::Checkbox::setFont(const sf::Font &font)
{
	this->boxCrosses[0].setFont(font);
	this->boxCrosses[1].setFont(font);
	this->label->setFont(font);
}

void FourWins::Menu::Checkbox::setCharacterSize(unsigned int size)
{
	this->boxCrosses[0].setCharacterSize(size);
	this->boxCrosses[1].setCharacterSize(size);
	this->label->setCharacterSize(size);
}

void FourWins::Menu::Checkbox::setYPos(float y)
{
	const sf::Vector2f &leftBoxPos = this->boxes[0].getPosition();
	const sf::Vector2f &rightBoxPos = this->boxes[1].getPosition();
	const sf::Vector2f &leftBoxCrossPos = this->boxCrosses[0].getPosition();
	const sf::Vector2f &rightBoxCrossPos = this->boxCrosses[1].getPosition();
	const sf::Vector2f &labelPos = this->label->getPosition();

	this->boxes[0].setPosition(sf::Vector2f(leftBoxPos.x, y));
	this->boxes[1].setPosition(sf::Vector2f(rightBoxPos.x, y));
	this->boxCrosses[0].setPosition(sf::Vector2f(leftBoxCrossPos.x, y));
	this->boxCrosses[1].setPosition(sf::Vector2f(rightBoxCrossPos.x, y));
	this->label->setPosition(sf::Vector2f(labelPos.x, y));
}

void FourWins::Menu::Checkbox::setLeftBoxXPos(float x)
{
	const sf::Vector2f &leftBoxPos = this->boxes[0].getPosition();
	const sf::Vector2f &leftBoxCrossPos = this->boxCrosses[0].getPosition();

	this->boxes[0].setPosition(sf::Vector2f(x, leftBoxPos.y));
	this->boxCrosses[0].setPosition(sf::Vector2f(x + 7.0f, leftBoxCrossPos.y));
}

void FourWins::Menu::Checkbox::setRightBoxXPos(float x)
{
	const sf::Vector2f &rightBoxPos = this->boxes[1].getPosition();
	const sf::Vector2f &rightBoxCrossPos = this->boxCrosses[1].getPosition();

	this->boxes[1].setPosition(sf::Vector2f(x, rightBoxPos.y));
	this->boxCrosses[1].setPosition(sf::Vector2f(x + 7.0f, rightBoxCrossPos.y));
}

void FourWins::Menu::Checkbox::setLabelXPos(float x)
{
	const sf::Vector2f &labelPos = this->label->getPosition();

	this->label->setPosition(sf::Vector2f(x, labelPos.y));
}

void FourWins::Menu::Checkbox::update(const sf::Event &e, const sf::Vector2f &mousePosition)
{
	for (unsigned int i = 0; i < 2; ++i)
	{
		this->boxes[i].setFillColor(Menu::BACKGROUND_COLOR);

		if (this->boxes[i].getGlobalBounds().contains(mousePosition))
		{
			switch (e.type)
			{
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseMoved:
				this->boxes[i].setFillColor(Menu::HIGHLIGHT_COLOR);
				break;
			case sf::Event::MouseButtonReleased:
				if (!this->boxCrosses[i].getString().isEmpty())
				{
					this->boxCrosses[i].setString("");
				}
				else
				{
					this->boxCrosses[i].setString("X");
				}
				this->boxes[i].setFillColor(Menu::HIGHLIGHT_COLOR);
				break;
			}
		}
	}
}

void FourWins::Menu::Checkbox::draw()
{
	if (this->isVisible[0])
	{
		this->window->draw(this->boxes[0]);
		this->window->draw(this->boxCrosses[0]);
	}

	if (this->isVisible[1])
	{
		this->window->draw(this->boxes[1]);
		this->window->draw(this->boxCrosses[1]);
	}

	if (this->isVisible[0] || this->isVisible[1])
	{
		this->window->draw(*this->label);
	}
}