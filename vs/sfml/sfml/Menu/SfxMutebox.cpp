#include <SFML/Graphics.hpp>
#include "../ResourceManager.h"
#include "../SoundManager.h"

#include "SfxMutebox.h"

SfxMutebox::SfxMutebox() :
	shape(new sf::RectangleShape()),
	textureRect(new sf::IntRect()),
	textureCheckedRect(new sf::IntRect()),
	soundManager(nullptr),
	isChecked(false)
{}

SfxMutebox::~SfxMutebox()
{
	delete this->shape;
	delete this->textureRect;
	delete this->textureCheckedRect;
}

void SfxMutebox::init(ResourceManager &resourceManager, SoundManager &soundManager)
{
	this->soundManager = &soundManager;

	const sf::IntRect sfxPlayRect = resourceManager.getTextureRect(resourceManager.SND_EFFECTS_PLAY);
	const sf::IntRect sfxMuteRect = resourceManager.getTextureRect(resourceManager.SND_EFFECTS_MUTE);

	this->shape->setTexture(resourceManager.getTexture(resourceManager.SOUND_SPRITE));
	this->textureRect->left = sfxPlayRect.left;
	this->textureRect->top = sfxPlayRect.top;
	this->textureRect->width = sfxPlayRect.width;
	this->textureRect->height = sfxPlayRect.height;
	this->textureCheckedRect->left = sfxMuteRect.left;
	this->textureCheckedRect->top = sfxMuteRect.top;
	this->textureCheckedRect->width = sfxMuteRect.width;
	this->textureCheckedRect->height = sfxMuteRect.height;
	this->shape->setTextureRect(*this->textureRect);
}

void SfxMutebox::setState(bool value)
{
	this->isChecked = value;
	updateTextureRect();
}

void SfxMutebox::setOrigin(const sf::Vector2f &origin)
{
	this->shape->setOrigin(origin);
}

void SfxMutebox::update(bool isButtonRelease, const sf::Vector2f &mousePosition)
{
	resetHover();
	if (this->shape->getGlobalBounds().contains(mousePosition))
	{
		hover();
		if (isButtonRelease)
		{
			this->isChecked = !this->isChecked;
			updateTextureRect();
		}
	}
}

void SfxMutebox::draw(sf::RenderWindow &window)
{
	window.draw(*this->shape);
}

void SfxMutebox::hover()
{
	this->shape->setFillColor(sf::Color::Magenta);
}

void SfxMutebox::resetHover()
{
	this->shape->setFillColor(sf::Color::White);
}

void SfxMutebox::updateTextureRect()
{
	if (this->isChecked)
	{
		//checked
		this->shape->setTextureRect(*this->textureCheckedRect);
		this->soundManager->setEffectsVolume(0.0f);
	}
	else
	{
		//not checked
		this->shape->setTextureRect(*this->textureRect);
		this->soundManager->setEffectsVolume(this->soundManager->VOLUME_EFFECTS);
	}
}