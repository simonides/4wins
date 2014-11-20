#include <SFML/Graphics.hpp>
#include "MenuConstants.h"

#include "Listbox.h"

using namespace FourWins;

const float FourWins::Menu::Listbox::ENTRY_HEIGHT = 40.0f;
const float FourWins::Menu::Listbox::LINE_TICKNESS = 1.0f;

FourWins::Menu::Listbox::Listbox(sf::RenderWindow &window) :
	window(&window),
	container(new sf::RectangleShape()),
	textsBB(new sf::RectangleShape[5]),
	texts(new sf::Text[5]),
	linesBetween(new sf::Vertex[8]),
	isEntryActive(new bool[5])
{}

FourWins::Menu::Listbox::~Listbox()
{
	delete this->container;
	delete[] this->textsBB;
	delete[] this->texts;
	delete[] this->linesBetween;
	delete[] this->isEntryActive;
}

void FourWins::Menu::Listbox::init()
{
	this->container->setSize(sf::Vector2f(200.0f, 204.0f));
	this->container->setFillColor(Menu::BACKGROUND_COLOR);
	this->container->setOutlineColor(Menu::OUTLINE_COLOR);
	this->container->setOutlineThickness(2.0f);

	for (unsigned int i = 0; i < 5; ++i)
	{
		this->texts[i].setColor(Menu::TEXT_COLOR);
		this->texts[i].setStyle(sf::Text::Bold);

		this->textsBB[i].setSize(sf::Vector2f(this->container->getSize().x, Listbox::ENTRY_HEIGHT));
		this->textsBB[i].setFillColor(sf::Color::Transparent);

		this->isEntryActive[i] = false;
	}
	this->texts[0].setString("Human Player");
	this->texts[1].setString("Intelligent AI");
	this->texts[2].setString("Random AI");
	this->texts[3].setString("Stupid AI");
	this->texts[4].setString("Thinking AI");

	for (unsigned int i = 0; i < 8; ++i)
	{
		this->linesBetween[i].color = Menu::OUTLINE_COLOR;
	}
}

void FourWins::Menu::Listbox::setFont(const sf::Font &font)
{
	this->texts[0].setFont(font);
	this->texts[1].setFont(font);
	this->texts[2].setFont(font);
	this->texts[3].setFont(font);
	this->texts[4].setFont(font);
}

void FourWins::Menu::Listbox::setPosition(const sf::Vector2f &position)
{
	const float indent = 5.0f;
	const float containerRightX = position.x + this->container->getSize().x;

	this->container->setPosition(position);

	for (unsigned int i = 0; i < 5; ++i)
	{
		float currentY = position.y + static_cast<float>(i) * (Listbox::ENTRY_HEIGHT + Listbox::LINE_TICKNESS);

		this->textsBB[i].setPosition(sf::Vector2f(position.x, currentY));
		this->texts[i].setPosition(sf::Vector2f(position.x + indent, currentY));

		if (i < 4u)
		{
			this->linesBetween[i * 2u].position = sf::Vector2f(position.x, currentY + Listbox::ENTRY_HEIGHT + 0.5f);
			this->linesBetween[i * 2u + 1u].position = sf::Vector2f(containerRightX, currentY + Listbox::ENTRY_HEIGHT + 0.5f);
		}
	}
}

const bool *FourWins::Menu::Listbox::getIsEntryActive() const
{
	return this->isEntryActive;
}

void FourWins::Menu::Listbox::update(const sf::Event &e, const sf::Vector2f &mousePosition)
{
	resetHover();

	if (this->container->getGlobalBounds().contains(mousePosition))
	{
		switch (e.type)
		{
		case sf::Event::MouseMoved:
			checkForHover(mousePosition);
			break;
		case sf::Event::MouseButtonReleased:
			checkForActive(mousePosition);
			break;
		}
	}
}

void FourWins::Menu::Listbox::draw()
{
	this->window->draw(*this->container);
	this->window->draw(this->textsBB[0]);
	this->window->draw(this->textsBB[1]);
	this->window->draw(this->textsBB[2]);
	this->window->draw(this->textsBB[3]);
	this->window->draw(this->textsBB[4]);
	this->window->draw(this->texts[0]);
	this->window->draw(this->texts[1]);
	this->window->draw(this->texts[2]);
	this->window->draw(this->texts[3]);
	this->window->draw(this->texts[4]);
	this->window->draw(this->linesBetween, 8, sf::Lines);
}

void FourWins::Menu::Listbox::resetHover()
{
	for (unsigned int i = 0; i < 5; ++i)
	{
		if (this->isEntryActive[i])
		{
			this->textsBB[i].setFillColor(Menu::HIGHLIGHT_COLOR);
		}
		else
		{
			this->textsBB[i].setFillColor(sf::Color::Transparent);
		}
	}
}

void FourWins::Menu::Listbox::checkForHover(const sf::Vector2f &mousePosition)
{
	for (unsigned int i = 0; i < 5; ++i)
	{
		if (this->textsBB[i].getGlobalBounds().contains(mousePosition))
		{
			this->textsBB[i].setFillColor(Menu::HIGHLIGHT_COLOR);
		}
		else
		{
			this->textsBB[i].setFillColor(sf::Color::Transparent);
		}
	}
}

void FourWins::Menu::Listbox::checkForActive(const sf::Vector2f &mousePosition)
{
	for (unsigned int i = 0; i < 5; ++i)
	{
		if (this->textsBB[i].getGlobalBounds().contains(mousePosition))
		{
			this->isEntryActive[i] = true;
		}
		else
		{
			this->isEntryActive[i] = false;
		}
	}
}