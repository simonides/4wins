#include "RGameMenu.h"
#include "config.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "RTextManager.h"
RGameMenu::RGameMenu(ResourceManager& resourceManager, SoundManager& soundManager, RTextManager& textManager)
	: GAME_MENU_BUTTON_SIZE(150.f, 150.f)
	, GAME_MENU_BUTTON_COLOR(sf::Color::White)
	, GAME_MENU_BUTTON_HOVER_COLOR(sf::Color::Magenta)
	, hoveredButtonPtr(nullptr)
	, exitButton(GAME_MENU_BUTTON_SIZE)
	, restartButton(GAME_MENU_BUTTON_SIZE)
	, menuButton(GAME_MENU_BUTTON_SIZE)
	, winner(GameWinner::TIE)
	, textManager(&textManager)
	, soundManager(&soundManager)
{

	//Game Menu Buttons:
	sf::Vector2f buttonOrigin(GAME_MENU_BUTTON_SIZE.x / 2.f, GAME_MENU_BUTTON_SIZE.y / 2.f);
	restartButton.setTexture(resourceManager.getTexture(ResourceManager::RELOAD_BTN_TEX));
	restartButton.setFillColor(GAME_MENU_BUTTON_COLOR);
	restartButton.setOrigin(buttonOrigin);
	restartButton.setPosition(WINDOW_WIDTH_TO_CALCULATE / 2.f - 150.f, WINDOW_HEIGHT_TO_CALCULATE / 2.f + 100.f);

	exitButton.setTexture(resourceManager.getTexture(ResourceManager::EXIT_BTN_TEX));
	exitButton.setFillColor(GAME_MENU_BUTTON_COLOR);
	exitButton.setOrigin(buttonOrigin);
	exitButton.setPosition(WINDOW_WIDTH_TO_CALCULATE / 2.f + 150.f, WINDOW_HEIGHT_TO_CALCULATE / 2.f + 100.f);

	menuButton.setTexture(resourceManager.getTexture(ResourceManager::MENU_BTN_TEX));
	menuButton.setFillColor(GAME_MENU_BUTTON_COLOR);
	menuButton.setOrigin(buttonOrigin);
	menuButton.setPosition(WINDOW_WIDTH_TO_CALCULATE / 2.f, WINDOW_HEIGHT_TO_CALCULATE / 2.f + 100.f);


	//TODO include soundbuttons.. 
	//constructor will take and resourcemanager soundmanager

}


RGameMenu::~RGameMenu()
{
}

void RGameMenu::reset()
{
	exitButton.setFillColor(GAME_MENU_BUTTON_COLOR);
	restartButton.setFillColor(GAME_MENU_BUTTON_COLOR);
	menuButton.setFillColor(GAME_MENU_BUTTON_COLOR);

	hoveredButtonPtr = nullptr;

}

void RGameMenu::draw(sf::RenderWindow& window)
{
	window.draw(exitButton);
	window.draw(restartButton);
	window.draw(menuButton);
	textManager->drawWinner(window, winner);


}

void RGameMenu::setMenuState(GameWinner::Enum winner)
{
	this->winner = winner;
}

void RGameMenu::resetHover()
{
	if (hoveredButtonPtr != nullptr){
		hoveredButtonPtr->setFillColor(GAME_MENU_BUTTON_COLOR);
		hoveredButtonPtr = nullptr;
	}
}

GameMenuDecision::Enum RGameMenu::handleClickAndHover(InputEvents* inputEvents)
{
	if (restartButton.getGlobalBounds().contains(inputEvents->mousePosition)){
		restartButton.setFillColor(GAME_MENU_BUTTON_HOVER_COLOR);
		hoveredButtonPtr = &restartButton;
		if (inputEvents->releasedLeftMouse){
			return GameMenuDecision::REPLAY;
		}
	} 
	else if (exitButton.getGlobalBounds().contains(inputEvents->mousePosition))
	{
		exitButton.setFillColor(GAME_MENU_BUTTON_HOVER_COLOR);
		hoveredButtonPtr = &exitButton;
		if (inputEvents->releasedLeftMouse){
			return GameMenuDecision::EXIT_GAME;
		}
	} 
	else if (menuButton.getGlobalBounds().contains(inputEvents->mousePosition))
	{
		menuButton.setFillColor(GAME_MENU_BUTTON_HOVER_COLOR);
		hoveredButtonPtr = &menuButton;
		if (inputEvents->releasedLeftMouse){
			soundManager->getMusic(SoundManager::SHEEP)->play();
			return GameMenuDecision::BACK_TO_MENU;
		}
	}
	return GameMenuDecision::KEEP_PLAYING;      //stay in the game loop until the user presses a button
}