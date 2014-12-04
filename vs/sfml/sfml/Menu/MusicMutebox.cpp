#include <SFML/Graphics.hpp>
#include "../ResourceManager.h"
#include "../SoundManager.h"

#include "MusicMutebox.h"

MusicMutebox::MusicMutebox() :
	shape(new sf::RectangleShape()),
	textureRect(new sf::IntRect()),
	textureCheckedRect(new sf::IntRect()),
	soundManager(nullptr),
	isChecked(false)
{}

MusicMutebox::~MusicMutebox()
{
	delete this->shape;
	delete this->textureRect;
	delete this->textureCheckedRect;
}

void MusicMutebox::init(ResourceManager &resourceManager, SoundManager &soundManager)
{
	this->soundManager = &soundManager;

	const sf::IntRect musicPlayRect = resourceManager.getTextureRect(resourceManager.SND_MUSIC_PLAY);
	const sf::IntRect musicMuteRect = resourceManager.getTextureRect(resourceManager.SND_MUSIC_MUTE);

	this->shape->setTexture(resourceManager.getTexture(resourceManager.SOUND_SPRITE));
	this->textureRect->left = musicPlayRect.left;
	this->textureRect->top = musicPlayRect.top;
	this->textureRect->width = musicPlayRect.width;
	this->textureRect->height = musicPlayRect.height;
	this->textureCheckedRect->left = musicMuteRect.left;
	this->textureCheckedRect->top = musicMuteRect.top;
	this->textureCheckedRect->width = musicMuteRect.width;
	this->textureCheckedRect->height = musicMuteRect.height;
	this->shape->setTextureRect(*this->textureRect);
}

void MusicMutebox::update(bool isButtonRelease, const sf::Vector2f &mousePosition)
{
	resetHover();
	if (this->shape->getGlobalBounds().contains(mousePosition))
	{
		hover();
		if (isButtonRelease)
		{
			switchState();
		}
	}
}

void MusicMutebox::draw(sf::RenderWindow &window)
{
	window.draw(*this->shape);
}

void MusicMutebox::hover()
{
	this->shape->setFillColor(sf::Color::Magenta);
}

void MusicMutebox::resetHover()
{
	this->shape->setFillColor(sf::Color::White);
}

void MusicMutebox::switchState()
{
	this->isChecked = !this->isChecked;

	if (this->isChecked)
	{
		//checked
		this->shape->setTextureRect(*this->textureCheckedRect);
		this->soundManager->setMusicVolume(0.0f);
	}
	else
	{
		//not checked
		this->shape->setTextureRect(*this->textureRect);
		this->soundManager->setMusicVolume(this->soundManager->VOLUME_BACKGROUND);
	}
}