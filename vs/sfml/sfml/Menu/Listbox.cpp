#include <SFML/Graphics.hpp>
#include "MenuConstants.h"
#include "ListboxEntry.h"

#include "Listbox.h"

using namespace FourWins;

const float FourWins::Menu::Listbox::ENTRY_HEIGHT = 34.0f;

FourWins::Menu::Listbox::Listbox(sf::RenderWindow &window, unsigned int entryCount) :
	window(&window),
	container(new sf::RectangleShape()),
	topBox(new sf::RectangleShape()),
	bottomBox(new sf::RectangleShape()),
	entries(new ListboxEntry[entryCount]),
	ENTRY_COUNT(entryCount)
{}

FourWins::Menu::Listbox::~Listbox()
{
	delete this->container;
	delete this->topBox;
	delete this->bottomBox;
	delete[] this->entries;
}

void FourWins::Menu::Listbox::init()
{
	const float containerWidth = 168.0f;
	const float containerHeight = Listbox::ENTRY_HEIGHT * static_cast<float>(this->ENTRY_COUNT);

	this->container->setSize(sf::Vector2f(containerWidth, containerHeight));
	this->container->setFillColor(Menu::BACKGROUND_COLOR);

	this->topBox->setSize(sf::Vector2f(208.0f, 30.0f));
	this->bottomBox->setSize(sf::Vector2f(208.0f, 30.0f));

	for (unsigned int i = 0; i < this->ENTRY_COUNT; ++i)
	{
		this->entries[i].setBackgroundColor(sf::Color::Transparent);
		this->entries[i].setTextColor(Menu::TEXT_COLOR);
		this->entries[i].setSize(sf::Vector2f(containerWidth, Listbox::ENTRY_HEIGHT));
		this->entries[i].setCharacterSize(24u);
	}
}

Menu::Listbox &FourWins::Menu::Listbox::setFont(const sf::Font &font)
{
	for (unsigned int i = 0; i < this->ENTRY_COUNT; ++i)
	{
		this->entries[i].setFont(font);
	}
	return *this;
}

Menu::Listbox &FourWins::Menu::Listbox::setPosition(const sf::Vector2f &position)
{
	this->container->setPosition(position);

	for (unsigned int i = 0; i < this->ENTRY_COUNT; ++i)
	{
		this->entries[i].setPosition(sf::Vector2f(position.x, position.y + static_cast<float>(i)* Listbox::ENTRY_HEIGHT));
	}

	const sf::Vector2f &boxSize = this->topBox->getSize();
	const sf::Vector2f &containerSize = this->container->getSize();

	this->topBox->setPosition(sf::Vector2f(position.x + (containerSize.x / 2) - (boxSize.x / 2), position.y - 22.0f));
	this->bottomBox->setPosition(sf::Vector2f(position.x + (containerSize.x / 2) - (boxSize.x / 2), position.y + containerSize.y - 7.0f));
	return *this;
}

Menu::Listbox &FourWins::Menu::Listbox::setTexture(const sf::Texture *texture)
{
	this->topBox->setTexture(texture);
	this->bottomBox->setTexture(texture);
	return *this;
}

Menu::Listbox &FourWins::Menu::Listbox::setValueForEntry(unsigned int index, unsigned char value)
{
	if (index < this->ENTRY_COUNT)
	{
		this->entries[index].setValue(value);
	}
	return *this;
}

Menu::Listbox &FourWins::Menu::Listbox::setStringForEntry(unsigned int index, const sf::String &text)
{
	if (index < this->ENTRY_COUNT)
	{
		this->entries[index].setString(text);
	}
	return *this;
}

Menu::Listbox &FourWins::Menu::Listbox::setTopTextureRect(const sf::IntRect &rect)
{
	this->topBox->setTextureRect(rect);
	return *this;
}

Menu::Listbox &FourWins::Menu::Listbox::setBottomTextureRect(const sf::IntRect &rect)
{
	this->bottomBox->setTextureRect(rect);
	return *this;
}

Menu::Listbox &FourWins::Menu::Listbox::setDefaultEntry(unsigned int index)
{
	if (index < this->ENTRY_COUNT)
	{
		for (unsigned int i = 0; i < this->ENTRY_COUNT; ++i)
		{
			this->entries[i].setIsActive(false);
		}
		this->entries[index].setIsActive(true);
	}
	resetHover();
	return *this;
}

unsigned char FourWins::Menu::Listbox::getValueOfActive() const
{
	for (unsigned int i = 0; i < this->ENTRY_COUNT; ++i)
	{
		if (this->entries[i].getIsActive())
		{
			return this->entries[i].getValue();
		}
	}
	return 0;
}

void FourWins::Menu::Listbox::update(const sf::Event &e, const sf::Vector2f &mousePosition)
{
	resetHover();

	if (this->container->getGlobalBounds().contains(mousePosition))
	{
		switch (e.type)
		{
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseMoved:
			checkForHover(mousePosition);
			break;
		case sf::Event::MouseButtonReleased:
			checkForActive(mousePosition);
			resetHover();
			break;
		}
	}
}

void FourWins::Menu::Listbox::draw()
{
	this->window->draw(*this->container);

	for (unsigned int i = 0; i < this->ENTRY_COUNT; ++i)
	{
		this->window->draw(this->entries[i].getShape());
		this->window->draw(this->entries[i].getText());
	}
	this->window->draw(*this->topBox);
	this->window->draw(*this->bottomBox);
}

void FourWins::Menu::Listbox::resetHover()
{
	for (unsigned int i = 0; i < this->ENTRY_COUNT; ++i)
	{
		if (this->entries[i].getIsActive())
		{
			this->entries[i].setBackgroundColor(Menu::HIGHLIGHT_COLOR);
		}
		else
		{
			this->entries[i].setBackgroundColor(sf::Color::Transparent);
		}
	}
}

void FourWins::Menu::Listbox::checkForHover(const sf::Vector2f &mousePosition)
{
	for (unsigned int i = 0; i < this->ENTRY_COUNT; ++i)
	{
		if (this->entries[i].getShape().getGlobalBounds().contains(mousePosition) && !this->entries[i].getIsActive())
		{
			this->entries[i].setBackgroundColor(Menu::HOVER_COLOR);
		}
	}
}

void FourWins::Menu::Listbox::checkForActive(const sf::Vector2f &mousePosition)
{
	for (unsigned int i = 0; i < this->ENTRY_COUNT; ++i)
	{
		this->entries[i].setIsActive(this->entries[i].getShape().getGlobalBounds().contains(mousePosition));
	}
}