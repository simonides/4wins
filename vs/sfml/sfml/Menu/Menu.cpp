#include <iostream>
#include <SFML/Graphics.hpp>

#include "../config.h"
#include "../helper.h"
#include "../ThinkingAI.h"
#include "../ResourceManager.h"
#include "MenuConstants.h"
#include "Listbox.h"
#include "Checkbox.h"
#include "Button.h"

#include "Menu.h"

using namespace FourWins;

FourWins::Menu::MainMenu::MainMenu(sf::RenderWindow &window) :
	window(&window),
	backgroundShape(new sf::RectangleShape()),
	headlineShape(new sf::RectangleShape()),
	labelPlayer1(new sf::Text()),
	labelPlayer2(new sf::Text()),
	lbPlayer1(new Menu::Listbox(window, 5u)),
	lbPlayer2(new Menu::Listbox(window, 5u)),
	cb(new Menu::Checkbox(window)),
	btnStart(new Menu::Button(window)),
	startGame(false)
{}

FourWins::Menu::MainMenu::~MainMenu()
{
	delete this->backgroundShape;
	delete this->headlineShape;
	delete this->labelPlayer1;
	delete this->labelPlayer2;
	delete this->lbPlayer1;
	delete this->lbPlayer2;
	delete this->btnStart;
}

void FourWins::Menu::MainMenu::init(ResourceManager &resourceManager)
{
	const sf::Texture *menuAtlas = resourceManager.getTexture(resourceManager.MENU_ATLAS);
	const sf::Font *font = resourceManager.getFont(resourceManager.ROBOTO);

	this->backgroundShape->setTexture(resourceManager.getTexture(resourceManager.BACKGROUND_TEX));
	this->backgroundShape->setTextureRect(resourceManager.getTextureRect(resourceManager.BACKGROUND));
	this->backgroundShape->setSize(sf::Vector2f(static_cast<float>(WINDOW_WIDTH_TO_CALCULATE), static_cast<float>(WINDOW_HEIGHT_TO_CALCULATE)));

	this->headlineShape->setTexture(menuAtlas);
	this->headlineShape->setTextureRect(sf::IntRect(0, 0, 500, 140));
	this->headlineShape->setSize(sf::Vector2f(500.0f, 140.0f));
	this->headlineShape->setPosition(sf::Vector2f(425.0f, 40.0f));

	this->labelPlayer1->setFont(*font);
	this->labelPlayer1->setString("Select player 1:");
	this->labelPlayer1->setCharacterSize(30u);
	this->labelPlayer1->setColor(Menu::LABEL_COLOR);
	this->labelPlayer1->setPosition(sf::Vector2f(417.0f, 200.0f));

	this->labelPlayer2->setFont(*font);
	this->labelPlayer2->setString("Select player 2:");
	this->labelPlayer2->setCharacterSize(30u);
	this->labelPlayer2->setColor(Menu::LABEL_COLOR);
	this->labelPlayer2->setPosition(sf::Vector2f(725.0f, 200.0f));

	this->lbPlayer1->init();
	this->lbPlayer1->setFont(*font);
	this->lbPlayer1->setTexture(menuAtlas);
	this->lbPlayer1->setTopTextureRect(resourceManager.getTextureRect(resourceManager.MENU_FRAME_UP));
	this->lbPlayer1->setBottomTextureRect(resourceManager.getTextureRect(resourceManager.MENU_FRAME_DOWN));
	this->lbPlayer1->setPosition(sf::Vector2f(437.0f, 275.0f));
	this->lbPlayer1->setStringForEntry(0, "Human Player");
	this->lbPlayer1->setValueForEntry(0, 'h');
	this->lbPlayer1->setStringForEntry(1u, "Intelligent AI");
	this->lbPlayer1->setValueForEntry(1u, 'i');
	this->lbPlayer1->setStringForEntry(2u, "Random AI");
	this->lbPlayer1->setValueForEntry(2u, 'r');
	this->lbPlayer1->setStringForEntry(3u, "Stupid AI");
	this->lbPlayer1->setValueForEntry(3u, 's');
	this->lbPlayer1->setStringForEntry(4u, "Thinking AI");
	this->lbPlayer1->setValueForEntry(4u, 't');

	this->lbPlayer2->init();
	this->lbPlayer2->setFont(*font);
	this->lbPlayer2->setTexture(menuAtlas);
	this->lbPlayer2->setTopTextureRect(resourceManager.getTextureRect(resourceManager.MENU_FRAME_UP));
	this->lbPlayer2->setBottomTextureRect(resourceManager.getTextureRect(resourceManager.MENU_FRAME_DOWN));
	this->lbPlayer2->setPosition(sf::Vector2f(745.0f, 275.0f));
	this->lbPlayer2->setStringForEntry(0, "Human Player");
	this->lbPlayer2->setValueForEntry(0, 'h');
	this->lbPlayer2->setStringForEntry(1u, "Intelligent AI");
	this->lbPlayer2->setValueForEntry(1u, 'i');
	this->lbPlayer2->setStringForEntry(2u, "Random AI");
	this->lbPlayer2->setValueForEntry(2u, 'r');
	this->lbPlayer2->setStringForEntry(3u, "Stupid AI");
	this->lbPlayer2->setValueForEntry(3u, 's');
	this->lbPlayer2->setStringForEntry(4u, "Thinking AI");
	this->lbPlayer2->setValueForEntry(4u, 't');

	this->cb->init();
	this->cb->setFont(*font);
	this->cb->setLabelText("Think");
	this->cb->setCharacterSize(24u);
	this->cb->setPosition(sf::Vector2f(100.0f, 100.0f));

	this->btnStart->init();
	this->btnStart->setTexture(menuAtlas);
	this->btnStart->setTextureRect(resourceManager.getTextureRect(resourceManager.MENU_STARTBTN));
	this->btnStart->setTextureHighlightRect(resourceManager.getTextureRect(resourceManager.MENU_STARTBTN_H));
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

		this->window->draw(*this->backgroundShape);
		this->window->draw(*this->labelPlayer1);
		this->window->draw(*this->labelPlayer2);
		this->lbPlayer1->draw();
		this->lbPlayer2->draw();
		this->cb->draw();
		this->btnStart->draw();
		this->window->draw(*this->headlineShape);

		this->window->display();
	}
	this->startGame = false;

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
		this->cb->update(event, converted);
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