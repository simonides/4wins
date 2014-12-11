#include "PreMenu.h"
#include "helper.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "config.h"
#include <sstream>
#include <iostream>
#include <assert.h>


PreMenu::PreMenu(sf::RenderWindow& window, ResourceManager& resourceManager, SoundManager& soundManager)
	: loopState(WAIT)
	, firstFadingFrame(false)
	, fadeIndexA(0)
	, fadeIndexB(1)
	, ANIMATION_SPEED(180)
	, TIME_TO_WAIT_TILL_NEXT_SWITCH(7.5f) // in seconds
	, alpha1(255)
	, alpha2(0)
	, window(&window)
	, resourceManager(&resourceManager)
	, soundManager(&soundManager)
	, goToMenu(false)
	, leftMouseClicked(false)
{
	float width = static_cast<float>(WINDOW_WIDTH_TO_CALCULATE);
	float height = static_cast<float>(WINDOW_HEIGHT_TO_CALCULATE);

	background[0].setPosition(0, 0);
	background[0].setSize(sf::Vector2f(width, height));
	background[0].setTexture(resourceManager.getTexture(ResourceManager::SPLASH_SCREEN));
	background[0].setTextureRect(resourceManager.getTextureRect(ResourceManager::FULL_SCREEN_RECT));
	background[0].setFillColor(sf::Color(255, 255, 255, 255));

	background[1].setPosition(0, 0);
	background[1].setSize(sf::Vector2f(width, height));
	background[1].setTexture(resourceManager.getTexture(ResourceManager::TUT1));
	background[1].setTextureRect(resourceManager.getTextureRect(ResourceManager::FULL_SCREEN_RECT));
	background[1].setFillColor(sf::Color(255, 255, 255, 0));

	background[2].setPosition(0, 0);
	background[2].setSize(sf::Vector2f(width, height));
	background[2].setTexture(resourceManager.getTexture(ResourceManager::TUT2));
	background[2].setTextureRect(resourceManager.getTextureRect(ResourceManager::FULL_SCREEN_RECT));
	background[2].setFillColor(sf::Color(255, 255, 255, 0));

	background[3].setPosition(0, 0);
	background[3].setSize(sf::Vector2f(width, height));
	background[3].setTexture(resourceManager.getTexture(ResourceManager::TUT3));
	background[3].setTextureRect(resourceManager.getTextureRect(ResourceManager::FULL_SCREEN_RECT));
	background[3].setFillColor(sf::Color(255, 255, 255, 0));

	background[4].setPosition(0, 0);
	background[4].setSize(sf::Vector2f(width, height));
	background[4].setTexture(resourceManager.getTexture(ResourceManager::TUT4));
	background[4].setTextureRect(resourceManager.getTextureRect(ResourceManager::FULL_SCREEN_RECT));
	background[4].setFillColor(sf::Color(255, 255, 255, 0));

	background[5].setPosition(0, 0);
	background[5].setSize(sf::Vector2f(width, height));
	background[5].setTexture(resourceManager.getTexture(ResourceManager::TUT5));
	background[5].setTextureRect(resourceManager.getTextureRect(ResourceManager::FULL_SCREEN_RECT));
	background[5].setFillColor(sf::Color(255, 255, 255, 0));

	background[6].setPosition(0, 0);
	background[6].setSize(sf::Vector2f(width, height));
	background[6].setTexture(resourceManager.getTexture(ResourceManager::BACKGROUND_TEX));
	background[6].setTextureRect(resourceManager.getTextureRect(ResourceManager::BACKGROUND));
	background[6].setFillColor(sf::Color(255, 255, 255, 0));
}


void PreMenu::runLoop()
{
	switchClock.restart();

	sf::Clock clock;
	float elapsedTime;
	float fpsElapsedTime = 0;
	
	while (window->isOpen() && goToMenu == false)
	{
		elapsedTime = clock.getElapsedTime().asSeconds();
		float fps = 1.f / elapsedTime;
		clock.restart();

		pollEvents();

		switch (loopState)
		{
		case CROSSFADE:
		{
			if (firstFadingFrame)
			{
				firstFadingFrame = false;
				alpha1 = 255;
				alpha2 = 0;
			}
			if (crossfade(elapsedTime, background[fadeIndexA], background[fadeIndexB])){
				loopState = WAIT;
				if (fadeIndexB == 6)
				{
					goToMenu = true;
					break;
				}
				++fadeIndexA;
				++fadeIndexB;
			}
		}break;
		
		case WAIT:
		{
			if (switchClock.getElapsedTime().asSeconds() > TIME_TO_WAIT_TILL_NEXT_SWITCH || leftMouseClicked)
			{
				leftMouseClicked = false;
				loopState = CROSSFADE;
				firstFadingFrame = true;
				switchClock.restart();
			}
		}break;

		default: assert(false);  break;
		}


		window->clear(sf::Color::White);
		
		window->draw(background[fadeIndexA]);
		window->draw(background[fadeIndexB]);
	
		window->display();

		fpsElapsedTime += elapsedTime;
		if (fpsElapsedTime > 0.75)
		{
			std::ostringstream ss;
			ss.precision(4);
			ss << WINDOW_TITLE << fps << "fps";
			window->setTitle(ss.str());
			fpsElapsedTime = 0;
		}
	}

	
}

bool PreMenu::crossfade(float elapsedTime, sf::RectangleShape& rect1, sf::RectangleShape& rect2)
{
	float temp = alpha1 - elapsedTime * ANIMATION_SPEED;
	float temp2 = alpha2 + elapsedTime * ANIMATION_SPEED;
	//std::cout << "color " << static_cast<int>(temp) << std::endl;
	//std::cout << "color2 " << static_cast<int>(temp2) << std::endl;
	
	// ensure high calc times don't screw up the outcome (drag the window for a sec outputs a too high value)
	// maybe just limit it would be better.. 
	if (temp < alpha1)
	{
		alpha1 = temp;
		alpha2 = temp2;
	}

	if (alpha1 < 1)
	{
		alpha1 = 0;
		alpha2 = 255;
		return true;
	}
	rect1.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha1)));
	rect2.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha2)));

	return false;
}

void PreMenu::reset()
{
	goToMenu = false;
	fadeIndexA = 0;
	fadeIndexB = 1;
}

void PreMenu::pollEvents()
{
	sf::Event event;

	while (window->pollEvent(event)){
		switch (event.type){

		case sf::Event::MouseButtonReleased:
			switch (event.mouseButton.button){
			case sf::Mouse::Left:
				leftMouseClicked = true;
				break;
			default: break;
			}
			break;
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::Escape)
			{
				//alpha1 = 255;			// fade to menubackground
				//alpha2 = 0;
				//fadeIndexB = 6;
				//loopState = CROSSFADE;
				goToMenu = true;		// go to menu directly
			}
			break;
		case sf::Event::Resized:
			handleResizeWindowEvent(window);
			break;

		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::LostFocus:
			//events.windowHasFocus = false;
			soundManager->getMusic(SoundManager::SHEEP)->play();
			break;
		case sf::Event::GainedFocus:
			//events.windowHasFocus = true;
			break;
		}
	}
}



