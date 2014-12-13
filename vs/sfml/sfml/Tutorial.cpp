#include "Tutorial.h"
#include "helper.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "config.h"
#include <sstream>
#include <iostream>
#include <assert.h>


Tutorial::Tutorial(sf::RenderWindow& window, ResourceManager& resourceManager, SoundManager& soundManager)
	: loopState(WAIT)
	, firstFadingFrame(false)
	, fadeIndexA(0)
	, fadeIndexB(1)
	, ANIMATION_SPEED(350)
	, TIME_TO_WAIT_TILL_NEXT_SWITCH(8.f) // in seconds
	, alpha1(255)
	, alpha2(0)
	, window(&window)
	, resourceManager(&resourceManager)
	, soundManager(&soundManager)
	, goToMenu(false)
	, leftMouseClicked(false)
	, rightMouseClicked(false)
{
	initBackground();
	background[0].setTexture(resourceManager.getTexture(ResourceManager::SPLASH_SCREEN));
	background[1].setTexture(resourceManager.getTexture(ResourceManager::TUT1));
	background[2].setTexture(resourceManager.getTexture(ResourceManager::TUT2));
	background[3].setTexture(resourceManager.getTexture(ResourceManager::TUT3));
	background[4].setTexture(resourceManager.getTexture(ResourceManager::TUT4));
	background[5].setTexture(resourceManager.getTexture(ResourceManager::TUT5));
	background[6].setTexture(resourceManager.getTexture(ResourceManager::TUT6));
	background[7].setTexture(resourceManager.getTexture(ResourceManager::TUT7));
	background[8].setTexture(resourceManager.getTexture(ResourceManager::TUT8));
	background[9].setTexture(resourceManager.getTexture(ResourceManager::TUT9));

	skipToMenuButton.setPosition(WINDOW_WIDTH_TO_CALCULATE - 250.f, 15.f);
	skipToMenuButton.setSize(sf::Vector2f(338.f * 0.7f , 73.f * 0.7f));
	skipToMenuButton.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	skipToMenuButton.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_SKIP_TO_MENU));
	skipToMenuButton.setFillColor(sf::Color::Black);
	
}


void Tutorial::runLoop()
{
	sf::Clock clock;
	float elapsedTime;
	float fpsElapsedTime = 0;

	switchClock.restart();
	
	while (window->isOpen() && goToMenu == false){
		elapsedTime = clock.getElapsedTime().asSeconds();
		float fps = 1.f / elapsedTime;
		clock.restart();

		pollEvents();

		if (skipToMenuButton.getGlobalBounds().contains(mousePosition))	{	//hovered

			skipToMenuButton.setFillColor(sf::Color(220,0,220,255));		//magenta
			if (leftMouseClicked)
			{
				leftMouseClicked = false;
				goToMenu = true;
				break;
			}
		} else {
			skipToMenuButton.setFillColor(sf::Color::Black);
		}

		switch (loopState)
		{
		case CROSSFADE:
		{
			leftMouseClicked = false;
			rightMouseClicked = false; // check if/why needed

			if (firstFadingFrame){
				firstFadingFrame = false;
				alpha1 = 255;
				alpha2 = 0;
			}
			if (crossfade(elapsedTime, background[fadeIndexA], background[fadeIndexB])){
				loopState = WAIT;
				bool goForward = fadeIndexB < fadeIndexA;
				if (goForward){
					uint8_t temp = fadeIndexA;		// swap indices
					fadeIndexA = fadeIndexB;
					fadeIndexB = temp;
				}
				else{
					++fadeIndexA;
					++fadeIndexB;
				}
			}
		} break;

		case WAIT:
		{
			if (rightMouseClicked)
			{
				rightMouseClicked = false;			// consume events
				leftMouseClicked = false;			// just to be safe
				switchClock.restart();

				if (fadeIndexB > 1){				// at second screen or higher
					firstFadingFrame = true;
					loopState = CROSSFADE;
					fadeIndexB -= 2;				// jump to one before fadeIndexA
				}
			}
			else if (switchClock.getElapsedTime().asSeconds() > TIME_TO_WAIT_TILL_NEXT_SWITCH || leftMouseClicked) {
				if (fadeIndexB == 10)								// 10 = backgroundarray size
				{
					goToMenu = true;
					break;
				}
				leftMouseClicked = false;							// consume events
				//rightMouseClicked = false;		// not needed because of the else if // just to be safe
				loopState = CROSSFADE;
				firstFadingFrame = true;
				switchClock.restart();
			}
		} break;

		default: { assert(false);					} break;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//draw
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		window->clear(sf::Color::White);
		
		window->draw(background[fadeIndexA]);
		window->draw(background[fadeIndexB]);
		if (fadeIndexA != 0 || loopState == CROSSFADE){
			window->draw(skipToMenuButton);
		} 

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

bool Tutorial::crossfade(float elapsedTime, sf::RectangleShape& rect1, sf::RectangleShape& rect2)
{
	float temp = alpha1 - elapsedTime * ANIMATION_SPEED;
	float temp2 = alpha2 + elapsedTime * ANIMATION_SPEED;
	// std::cout << "color " << static_cast<int>(temp) << std::endl;
	// std::cout << "color2 " << static_cast<int>(temp2) << std::endl;
	
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
		rect1.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha1)));
		return true;
	}
	// disabled because it should not be crossfaded only overlayed 
	//rect1.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha1)));
	rect2.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha2)));
	return false;
}

void Tutorial::initBackground(){
	float width = static_cast<float>(WINDOW_WIDTH_TO_CALCULATE);
	float height = static_cast<float>(WINDOW_HEIGHT_TO_CALCULATE);

	for (uint8_t i = 0; i < 10; ++i){
		background[i].setPosition(0, 0);
		background[i].setSize(sf::Vector2f(width, height));
		background[i].setTextureRect(resourceManager->getTextureRect(ResourceManager::FULL_SCREEN_RECT));
		background[i].setFillColor(sf::Color(255, 255, 255, 0));
	}
	background[0].setFillColor(sf::Color(255, 255, 255, 255));
}
void Tutorial::reset()
{
	goToMenu = false;
	fadeIndexA = 1;
	fadeIndexB = 2;
	initBackground();
	background[1].setFillColor(sf::Color(255, 255, 255, 255)); // set second screen opaqe
}

void Tutorial::skipToMenu()
{
	//alpha1 = 255;			// fade to menubackground
	//alpha2 = 0;
	//fadeIndexB = 6;
	//loopState = CROSSFADE;ect1
	goToMenu = true;		// go to menu directly
}
void Tutorial::pollEvents()
{
	sf::Event event;
	mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	leftMouseClicked = false;
	rightMouseClicked = false;

	while (window->pollEvent(event)){
		switch (event.type){

		case sf::Event::MouseButtonReleased:
		{
			switch (event.mouseButton.button){
			case sf::Mouse::Left:{		leftMouseClicked = true;	} break;
			case sf::Mouse::Right:{		rightMouseClicked = true;	} break;
			default:{ /* do nothing */								} break;
			}
		} break;

		case sf::Event::KeyReleased:{
			if (event.key.code == sf::Keyboard::Escape){
				skipToMenu();
			}
			else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Space){
				leftMouseClicked = true;								// mouse is the main input and this can be reused
			}
			else if (event.key.code == sf::Keyboard::Left){
				rightMouseClicked = true;								// mouse is the main input and this can be reused
			}
		} break;

		case sf::Event::Resized:	{	handleResizeWindowEvent(window);	} break;
		case sf::Event::Closed:		{	window->close();					} break;
		case sf::Event::LostFocus:	{
			//events.windowHasFocus = false;
			soundManager->getMusic(SoundManager::SHEEP)->play();
		} break;
		case sf::Event::GainedFocus: { /*events.windowHasFocus = true;*/	} break;
		}
	}
}



