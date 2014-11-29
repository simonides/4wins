#include "RTextManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "ResourceManager.h"
#include "GameState.h"
#include <assert.h>

#include "config.h"

#include <math.h>

#define PI 3.14159265
#include <iostream>


const float SIZE_FAKTOR = 1.f;
const float SIZE_FAKTOR_END = 0.8f;
RTextManager::RTextManager(ResourceManager& resourceManager)
	: chooseX(145.f), arrowX(120.f), selectX(120.f), positionX(180.f), meepleX(170.f)
{
	player1.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	player1.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_PLAYER_1));
	player1.setSize(sf::Vector2f(762.f *SIZE_FAKTOR_END, 212.f *SIZE_FAKTOR_END));
	player1.setOrigin(player1.getSize().x / 2.f, player1.getSize().y / 2.f);
	player1.setPosition(440.f, 250.f);


	player2.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	player2.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_PLAYER_2));
	player2.setSize(sf::Vector2f(762.f *SIZE_FAKTOR_END, 212.f *SIZE_FAKTOR_END));
	player2.setOrigin(player2.getSize().x / 2.f, player2.getSize().y / 2.f);
	player2.setPosition(440.f, 250.f);

	wins.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	wins.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_WINS));
	wins.setSize(sf::Vector2f(581.f *SIZE_FAKTOR_END, 210.f * SIZE_FAKTOR_END));
	wins.setOrigin(wins.getSize().x / 2.f, wins.getSize().y / 2.f);
	wins.setPosition(1010.f, 235.f);

	tie.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	tie.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_TIE));
	tie.setSize(sf::Vector2f(441.f *SIZE_FAKTOR_END, 210.f * SIZE_FAKTOR_END));
	tie.setOrigin(tie.getSize().x / 2.f, 0.f);
	tie.setPosition(WINDOW_WIDTH_TO_CALCULATE / 2.f, 200.f);

	pause.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	pause.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_PAUSE));
	pause.setSize(sf::Vector2f(612.f *SIZE_FAKTOR_END, 210.f *SIZE_FAKTOR_END));
	pause.setOrigin(pause.getSize().x / 2.f, pause.getSize().y / 2.f);
	pause.setPosition(WINDOW_WIDTH_TO_CALCULATE / 2.f, 200.f);

	arrowUp.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	arrowUp.setTextureRect(resourceManager.getTextureRect(ResourceManager::ARROW_UP));
	arrowUp.setSize(sf::Vector2f(131.f *SIZE_FAKTOR, 243.f *SIZE_FAKTOR));
	arrowUp.setOrigin(arrowUp.getSize().x / 2.f, arrowUp.getSize().y / 2.f);

	choose.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	choose.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_CHOOSE));
	choose.setSize(sf::Vector2f(241.f *SIZE_FAKTOR, 73.f *SIZE_FAKTOR));
	choose.setOrigin(choose.getSize().x / 2.f, choose.getSize().y / 2.f);

	meeple.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	meeple.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_A_MEEPLE));
	meeple.setFillColor(sf::Color::Black);
	//meeple.setOutlineColor(sf::Color::Red);
	//meeple.setOutlineThickness(5.f);
	meeple.setSize(sf::Vector2f(291.f *SIZE_FAKTOR, 73.f *SIZE_FAKTOR));
	meeple.setOrigin(meeple.getSize().x / 2.f, meeple.getSize().y / 2.f);


	select.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	select.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_SELECT));
	select.setSize(sf::Vector2f(203.f *SIZE_FAKTOR, 73.f *SIZE_FAKTOR));
	select.setOrigin(select.getSize().x / 2.f, select.getSize().y / 2.f);

	position.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	position.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_A_POSITION));
	position.setSize(sf::Vector2f(324.f *SIZE_FAKTOR, 73.f *SIZE_FAKTOR));
	position.setOrigin(position.getSize().x / 2.f, position.getSize().y / 2.f);



	ANIMATIONSPEED = 3.f;// 3.f;

	for (uint8_t i = 0; i < 2; i++)
	{
		wobbleNR = 0.f;
		wobbleSin[i] = 0.f;
	}
}


RTextManager::~RTextManager()
{
}

void RTextManager::update(float elapsedTime)
{
	for (uint8_t i = 0; i < 2; i++)
	{
		wobbleNR += ANIMATIONSPEED * elapsedTime;
		if (wobbleNR > 2 * PI) 
		{
			wobbleNR = 0;
		}
		wobbleSin[i] = (i % 2 == 0) ? 1.f + 0.035f * - static_cast<float>(sin(wobbleNR)) : 1 + 0.09f * static_cast<float>(sin(wobbleNR)); // width : height
	}

	choose.setScale(	wobbleSin[0], wobbleSin[1]);
	arrowUp.setScale(	wobbleSin[0], wobbleSin[1]);
	meeple.setScale(	wobbleSin[0], wobbleSin[1]);
	select.setScale(	wobbleSin[0], wobbleSin[1]);
	position.setScale(	wobbleSin[0], wobbleSin[1]);
	player1.setScale(	wobbleSin[0], wobbleSin[1]);
	player2.setScale(	wobbleSin[0], wobbleSin[1]);
	wins.setScale(		wobbleSin[0], wobbleSin[1]);
	tie.setScale(		wobbleSin[0], wobbleSin[1]);
	pause.setScale(		wobbleSin[0], wobbleSin[1]);
}

void RTextManager::drawWinner(sf::RenderWindow& window, GameWinner::Enum winner) const
{
	switch (winner)
	{
	case GameWinner::PLAYER_1:
		window.draw(player1);  
		window.draw(wins);
		break;
	case GameWinner::PLAYER_2:
		window.draw(player2);
		window.draw(wins);
		break;
	case GameWinner::TIE: 
		window.draw(tie); 
		break;
	case GameWinner::PAUSE:
		window.draw(pause);
		break;
	default: assert(false); break;
	}
	
}



void RTextManager::drawTodo(sf::RenderWindow& window, GameAction action, uint8_t player)
{

	if (player == 0) // player 1
	{
		select.setPosition(selectX, 540.f);
		choose.setPosition(chooseX, 540.f);
		arrowUp.setPosition(WINDOW_WIDTH_TO_CALCULATE - arrowX, 560.f);
		position.setPosition(positionX, 620.f);
		meeple.setPosition(meepleX, 620.f);
	} else if (player == 1) // player 2
	{
		select.setPosition(WINDOW_WIDTH_TO_CALCULATE - selectX, 540.f);
		choose.setPosition(WINDOW_WIDTH_TO_CALCULATE - chooseX, 540.f);
		arrowUp.setPosition(arrowX, 560.f);
		position.setPosition(WINDOW_WIDTH_TO_CALCULATE - positionX, 620.f);
		meeple.setPosition(WINDOW_WIDTH_TO_CALCULATE - meepleX, 620.f);
	}
	switch (action)
	{
	case SELECT_MEEPLE_POS: 
		window.draw(select);
		//window.draw(arrowUp);
		window.draw(position);
		break;
	case CHOOSE_A_MEEPLE:
		window.draw(choose); 
		window.draw(arrowUp);
		window.draw(meeple);
		break;
	default: assert(false); break;
	}
}





