#include <SFML/Graphics.hpp>
#include "MenuConstants.h"

#include "Button.h"

using namespace FourWins;

FourWins::Menu::Button::Button(sf::RenderWindow &window) :
	window(&window),
	background(new sf::RectangleShape()),
	text(new sf::Text()),
	isReleased(false)
{}

FourWins::Menu::Button::~Button()
{
	delete this->background;
	delete this->text;
}

void FourWins::Menu::Button::init()
{
	this->background->setSize(sf::Vector2f(278.0f, 50.0f));
	this->background->setFillColor(Menu::BACKGROUND_COLOR);
	this->background->setOutlineColor(Menu::OUTLINE_COLOR);
	this->background->setOutlineThickness(2.0f);
	this->background->setPosition(sf::Vector2f(512.0f, 470.0f));

	this->text->setColor(Menu::TEXT_COLOR);
	this->text->setCharacterSize(36u);
	this->text->setStyle(sf::Text::Bold);
	this->text->setString("Start new game");
	this->text->setPosition(sf::Vector2f(520.0f, 471.0f));
}


void FourWins::Menu::Button::setFont(const sf::Font &font)
{
	this->text->setFont(font);
}

void FourWins::Menu::Button::setPosition(const sf::Vector2f &position)
{
	this->background->setPosition(position);

	sf::FloatRect textBB = this->text->getGlobalBounds();
	const sf::Vector2f &backgroundSize = this->background->getSize();
	
	this->text->setPosition(sf::Vector2f(
		position.x + backgroundSize.x - textBB.width / 2,
		position.y + backgroundSize.y - textBB.height / 2));
}

void FourWins::Menu::Button::setSize(const sf::Vector2f &size)
{
	this->background->setSize(size);

	sf::FloatRect textBB = this->text->getGlobalBounds();
	const sf::Vector2f &backgroundPosition = this->background->getPosition();

	this->text->setPosition(sf::Vector2f(
		backgroundPosition.x + size.x - textBB.width / 2,
		backgroundPosition.y + size.y - textBB.height / 2));
}

void FourWins::Menu::Button::setText(const sf::String &text)
{
	this->text->setString(text);
}

void FourWins::Menu::Button::setCharacterSize(unsigned int size)
{
	this->text->setCharacterSize(size);
}

void FourWins::Menu::Button::setAlpha(sf::Uint8 alpha)
{
	const sf::Color &backgroundColor = this->background->getFillColor();
	const sf::Color &textColor = this->text->getColor();

	this->background->setFillColor(sf::Color(backgroundColor.r, backgroundColor.g, backgroundColor.b, alpha));
	this->text->setColor(sf::Color(textColor.r, textColor.g, textColor.b, alpha));
}

bool FourWins::Menu::Button::getIsReleased() const
{
	return this->isReleased;
}

void FourWins::Menu::Button::update(const sf::Event &e, const sf::Vector2f &mousePosition)
{
	this->isReleased = false;
	this->background->setFillColor(Menu::BACKGROUND_COLOR);
	
	if (this->background->getGlobalBounds().contains(mousePosition))
	{
		switch (e.type)
		{
		case sf::Event::MouseMoved:
			this->background->setFillColor(Menu::HIGHLIGHT_COLOR);
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
	this->window->draw(*this->text);
}