#include "MenuConstants.h"
#include "../config.h"
#include "../ThinkingAI.h"
#include "Listbox.h"
#include "Button.h"
#include "../helper.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Menu.h"

using namespace FourWins;

FourWins::Menu::MainMenu::MainMenu(sf::RenderWindow &window) :
	window(&window),
	font(new sf::Font()),
	labelHeader(new sf::Text()),
	labelPlayer1(new sf::Text()),
	labelPlayer2(new sf::Text()),
	lbPlayer1(new Menu::Listbox(window)),
	lbPlayer2(new Menu::Listbox(window)),
	btnStart(new Menu::Button(window)),
	startGame(false)
{}

FourWins::Menu::MainMenu::~MainMenu()
{
	delete this->font;
	delete this->labelHeader;
	delete this->labelPlayer1;
	delete this->labelPlayer2;
	delete this->lbPlayer1;
	delete this->lbPlayer2;
	delete this->btnStart;
}

void FourWins::Menu::MainMenu::init()
{
	if (!this->font->loadFromFile(WORKING_DIR + "Fonts/roboto/Roboto-Thin.ttf")){
		std::cerr << "error: menu font missing." << std::endl;
		exit(-1);
	}

	this->labelHeader->setFont(*this->font);
	this->labelHeader->setString("Menu");
	this->labelHeader->setCharacterSize(120u); // in pixels, not points!
	this->labelHeader->setColor(Menu::HEADLINE_COLOR);
	this->labelHeader->setStyle(sf::Text::Bold);
	this->labelHeader->setPosition(sf::Vector2f(500.0f, 40.0f));

	this->labelPlayer1->setFont(*this->font);
	this->labelPlayer1->setString("Select player 1:");
	this->labelPlayer1->setCharacterSize(30u);
	this->labelPlayer1->setColor(Menu::TEXT_COLOR);
	this->labelPlayer1->setPosition(sf::Vector2f(400.0f, 200.0f));

	this->labelPlayer2->setFont(*this->font);
	this->labelPlayer2->setString("Select player 2:");
	this->labelPlayer2->setCharacterSize(30u);
	this->labelPlayer2->setColor(Menu::TEXT_COLOR);
	this->labelPlayer2->setPosition(sf::Vector2f(700.0f, 200.0f));

	this->lbPlayer1->init();
	this->lbPlayer1->setFont(*this->font);
	this->lbPlayer1->setPosition(sf::Vector2f(395.0f, 245.0f));

	this->lbPlayer2->init();
	this->lbPlayer2->setFont(*this->font);
	this->lbPlayer2->setPosition(sf::Vector2f(695.0f, 245.0f));

	this->btnStart->init();
	this->btnStart->setFont(*this->font);
}

GameSettings FourWins::Menu::MainMenu::loop()
{
	startGame = false;
	while (this->window->isOpen() && !this->startGame)
	{
		pollEvents();

		if (this->btnStart->getIsReleased())
		{
			this->startGame = true;
		}

		this->window->clear(sf::Color::Black);

		this->window->draw(*this->labelHeader);
		this->window->draw(*this->labelPlayer1);
		this->window->draw(*this->labelPlayer2);
		this->lbPlayer1->draw();
		this->lbPlayer2->draw();
		this->btnStart->draw();

		this->window->display();
	}

	return createSettings();
}

void FourWins::Menu::MainMenu::pollEvents()
{
	sf::Event event;
	
	while (this->window->pollEvent(event))
	{
		sf::Vector2i mousepos = sf::Mouse::getPosition(*this->window);
		sf::Vector2f converted = this->window->mapPixelToCoords(mousepos);

		this->lbPlayer1->update(event, converted);
		this->lbPlayer2->update(event, converted);
		this->btnStart->update(event, converted);

		switch (event.type)
		{
		case sf::Event::Resized:
			handleResizeWindowEvent(window);
			break;

		case sf::Event::Closed:
			this->window->close();
			break;
		}
	}
}

GameSettings FourWins::Menu::MainMenu::createSettings()
{
	I_Player* thinking = new ThinkingAI(true, true);

	GameSettings settings;
	settings.playerOne = thinking;
	settings.playerTwo = thinking;

	return settings;
}
//if (event.type == sf::Event::KeyPressed){
//if (event.key.code == sf::Keyboard::K){	}
//}
//if (event.type == sf::Event::KeyReleased){
//if (event.key.code == sf::Keyboard::K){	}
//}
//if (event.type == sf::Event::MouseWheelMoved){	}

//if (event.type == sf::Event::MouseButtonPressed)
//{
//if (event.mouseButton.button == sf::Mouse::Left){
//std::cout << "mouse x: " << event.mouseButton.x << std::endl;
//std::cout << "mouse y: " << event.mouseButton.y << std::endl;
//}
//if (event.mouseButton.button == sf::Mouse::Right){
//std::cout << "the right button was pressed" << std::endl;
//}
//}