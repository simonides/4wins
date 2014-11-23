#include "RBackground.h"
#include "config.h"
#include "Game.h"

const float BACKGRND_WINDOW_WIDTH = 320.f;
const float BACKGRND_WINDOW_HEIGHT = 490.f;

const float AVATAR_WIDTH = BACKGRND_WINDOW_WIDTH;
const float AVATAR_HEIGHT = BACKGRND_WINDOW_HEIGHT;

const float ANIMATION_SPEED = 250.f;

const float WINDOW_POSITION_FROM_TOP = 13.f;
const float LEFT_WINDOW_POSITION_FROM_LEFT = 35.f;
const float RIGHT_WINDOW_POSITION_FROM_LEFT = 986.f;

RBackground::RBackground(ResourceManager& resourceLoader, Player& p1, Player& p2)
	: closeLeftWindow(true), closeRightWindow(true)
{
	background.setSize(sf::Vector2f(static_cast<float>(WINDOW_WIDTH_TO_CALCULATE), static_cast<float>(WINDOW_HEIGHT_TO_CALCULATE)));
	background.setTexture(resourceLoader.getTexture(ResourceManager::BACKGROUND_TEX));
	background.setTextureRect(resourceLoader.getTextureRect(ResourceManager::BACKGROUND));
	background.setPosition(0.f, 0.f);


	leftWindow.setSize(sf::Vector2f(BACKGRND_WINDOW_WIDTH, BACKGRND_WINDOW_HEIGHT));
	leftWindow.setTexture(resourceLoader.getTexture(ResourceManager::BACKGROUND_TEX));
	leftWindow.setTextureRect(resourceLoader.getTextureRect(ResourceManager::BACKGROUND_WINDOW));
	//leftWindow.setFillColor(sf::Color(255, 0, 0, 80));
	leftWindow.setPosition(LEFT_WINDOW_POSITION_FROM_LEFT, WINDOW_POSITION_FROM_TOP);

	rightWindow.setSize(sf::Vector2f(BACKGRND_WINDOW_WIDTH, BACKGRND_WINDOW_HEIGHT));
	rightWindow.setTexture(resourceLoader.getTexture(ResourceManager::BACKGROUND_TEX));
	rightWindow.setTextureRect(resourceLoader.getTextureRect(ResourceManager::BACKGROUND_WINDOW));
	//rightWindow.setFillColor(sf::Color(255,0,0,80));
	rightWindow.setPosition(RIGHT_WINDOW_POSITION_FROM_LEFT, WINDOW_POSITION_FROM_TOP);

	avatarPlayer1.setSize(sf::Vector2f(AVATAR_WIDTH, AVATAR_HEIGHT));
	avatarPlayer1.setTexture(resourceLoader.getTexture(ResourceManager::AVATAR_SPRITE));
	avatarPlayer1.setTextureRect(resourceLoader.getTextureRect(p1.playerAvatar));
	avatarPlayer1.setPosition(LEFT_WINDOW_POSITION_FROM_LEFT, WINDOW_POSITION_FROM_TOP);

	avatarPlayer2.setSize(sf::Vector2f(AVATAR_WIDTH, AVATAR_HEIGHT));
	avatarPlayer2.setTexture(resourceLoader.getTexture(ResourceManager::AVATAR_SPRITE));
	avatarPlayer2.setTextureRect(resourceLoader.getTextureRect(p2.playerAvatar));
	avatarPlayer2.setPosition(RIGHT_WINDOW_POSITION_FROM_LEFT, WINDOW_POSITION_FROM_TOP);
	
}


RBackground::~RBackground()
{
}


void RBackground::draw(sf::RenderWindow& window) const
{
	window.draw(avatarPlayer1);
	window.draw(avatarPlayer2);
	window.draw(leftWindow);
	window.draw(rightWindow);
	window.draw(background);
}


void  RBackground::setLeftWindow(bool closeWindow)
{
	closeLeftWindow = closeWindow;
}

void  RBackground::setRightWindow(bool closeWindow)
{
	closeRightWindow = closeWindow;
}


void  RBackground::update(float elapsedTime)
{
	//left Window
	//close
	if (closeLeftWindow && leftWindow.getPosition().x > LEFT_WINDOW_POSITION_FROM_LEFT )
	{
		leftWindow.move(-(ANIMATION_SPEED * elapsedTime), 0.f);
	}
	//open
	else if (!closeLeftWindow && leftWindow.getPosition().x < LEFT_WINDOW_POSITION_FROM_LEFT + BACKGRND_WINDOW_WIDTH)
	{
		leftWindow.move(ANIMATION_SPEED * elapsedTime, 0.f);
	}

	//right Window
	//close
	if (closeRightWindow && rightWindow.getPosition().x < RIGHT_WINDOW_POSITION_FROM_LEFT)
	{
		rightWindow.move(ANIMATION_SPEED * elapsedTime, 0.f);

	}
	//open
	else if (!closeRightWindow &&  rightWindow.getPosition().x > RIGHT_WINDOW_POSITION_FROM_LEFT - BACKGRND_WINDOW_WIDTH)
	{
		rightWindow.move(-(ANIMATION_SPEED * elapsedTime), 0.f);
	}

}
