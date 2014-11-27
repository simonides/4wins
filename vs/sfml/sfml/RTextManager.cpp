#include "RTextManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "ResourceManager.h"
#include "GameState.h"
#include <assert.h>

#include "config.h"

#include <math.h>

#define PI 3.14159265
#include <iostream>


const float SIZE_FAKTOR = 0.4f;
RTextManager::RTextManager(ResourceManager& resourceManager)
{
	white.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	white.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_WHITE));
	white.setSize(sf::Vector2f(500.f, 122.f));
	white.setOrigin(white.getSize().x / 2.f, white.getSize().y / 2.f);
	white.setPosition(350.f, 250.f);


	black.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	black.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_BLACK));
	black.setSize(sf::Vector2f(496.f, 127.f));
	black.setOrigin(black.getSize().x / 2.f, black.getSize().y / 2.f);
	black.setPosition(350.f, 250.f);

	wins.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	wins.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_WINS));
	wins.setSize(sf::Vector2f(421.f, 117.f));
	wins.setOrigin(wins.getSize().x / 2.f, wins.getSize().y / 2.f);
	wins.setPosition(920.f, 250.f);

	tie.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	tie.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_TIE));
	tie.setSize(sf::Vector2f(329.f, 117.f));
	tie.setOrigin(tie.getSize().x / 2.f, 0.f);
	tie.setPosition(WINDOW_WIDTH_TO_CALCULATE / 2.f, 200.f);

	choose.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	choose.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_CHOOSE));
	choose.setSize(sf::Vector2f(607.f *SIZE_FAKTOR, 123.f *SIZE_FAKTOR));
	choose.setOrigin(choose.getSize().x / 2.f, choose.getSize().y / 2.f);

	a.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	a.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_A));
	a.setSize(sf::Vector2f(159.f *SIZE_FAKTOR, 117.f *SIZE_FAKTOR));
	a.setOrigin(a.getSize().x / 2.f, a.getSize().y / 2.f);


	meeple.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	meeple.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_MEEPLE));
	meeple.setSize(sf::Vector2f(529.f *SIZE_FAKTOR, 123.f *SIZE_FAKTOR));
	meeple.setOrigin(meeple.getSize().x / 2.f, meeple.getSize().y / 2.f);


	select.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	select.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_SELECT));
	select.setSize(sf::Vector2f(598.f *SIZE_FAKTOR, 123.f *SIZE_FAKTOR));
	select.setOrigin(select.getSize().x / 2.f, select.getSize().y / 2.f);

	position.setTexture(resourceManager.getTexture(ResourceManager::TEXT_SPRITE));
	position.setTextureRect(resourceManager.getTextureRect(ResourceManager::TEXT_POSITION));
	position.setSize(sf::Vector2f(767.f *SIZE_FAKTOR, 122.f *SIZE_FAKTOR));
	position.setOrigin(position.getSize().x / 2.f, position.getSize().y / 2.f);

	ANIMATIONSPEED = 3.f;

	for (uint8_t i = 0; i < 18; i++)
	{
		wobbleNR[i] = 6.f / 18.f * i; // 0-6
		wobbleSin[i] = 0.f;
	}
}


RTextManager::~RTextManager()
{
}

void RTextManager::update(float elapsedTime)
{
	for (uint8_t i = 0; i < 18; i++)
	{
		wobbleNR[i] += ANIMATIONSPEED * elapsedTime;
		if (wobbleNR[i] > 2 * PI) 
		{
			wobbleNR[i] = 0;
		}
		wobbleSin[i] = (i % 2 == 0) ? 1.1f + 0.04 * sin(wobbleNR[i]) : 1 + 0.05 * sin(wobbleNR[i]); // width : height
	}

	choose.setScale(	wobbleSin[0], wobbleSin[1]);
	a.setScale(			wobbleSin[2], wobbleSin[3]);
	meeple.setScale(	wobbleSin[4], wobbleSin[5]);
	select.setScale(	wobbleSin[6], wobbleSin[7]);
	position.setScale(	wobbleSin[8], wobbleSin[9]);
	white.setScale(		wobbleSin[10], wobbleSin[11]);
	black.setScale(		wobbleSin[12], wobbleSin[13]);
	wins.setScale(		wobbleSin[14], wobbleSin[15]);
	tie.setScale(		wobbleSin[16], wobbleSin[17]);
}

void RTextManager::drawWinner(sf::RenderWindow& window, GameWinner::Enum winner) const
{
	switch (winner)
	{
	case GameWinner::PLAYER_1:
		window.draw(white);  
		window.draw(wins);
		break;
	case GameWinner::PLAYER_2:
		window.draw(black);
		window.draw(wins);
		break;
	case GameWinner::TIE: 
		window.draw(tie); 
		break;
	default: assert(false); break;
	}
	
}

void RTextManager::drawTodo(sf::RenderWindow& window, GameAction action, uint8_t player)
{
	if (player == 0) // player 1
	{
		select.setPosition(145.f, 520.f);
		choose.setPosition(145.f, 520.f);
		a.setPosition(55.f, 600.f);
		position.setPosition(250.f, 600.f);
		meeple.setPosition(225.f, 600.f);
	} else if (player == 1) // player 2
	{
		select.setPosition(1200.f, 520.f);
		choose.setPosition(1200.f, 520.f);
		a.setPosition(970.f, 600.f);
		position.setPosition(1170.f, 600.f);
		meeple.setPosition(1210.f, 600.f);
	}
	switch (action)
	{
	case SELECT_MEEPLE_POS: 
		window.draw(select);
		window.draw(a);
		window.draw(position);
		break;
	case CHOOSE_A_MEEPLE:
		if (player == 1){
			a.setPosition(1050.f, 600.f);
		}
		window.draw(choose); 
		window.draw(a);
		window.draw(meeple);
		break;
	default: assert(false); break;
	}
}





