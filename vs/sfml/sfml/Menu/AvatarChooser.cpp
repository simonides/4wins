#include <SFML/Graphics.hpp>
#include "MenuConstants.h"

#include "AvatarChooser.h"

using namespace FourWins;

const float FourWins::Menu::AvatarChooser::THUMB_WIDTH = 56.0f;
const float FourWins::Menu::AvatarChooser::THUMB_HEIGHT = 85.0f;
const float FourWins::Menu::AvatarChooser::THUMB_SPACE = 2.0f;
const float FourWins::Menu::AvatarChooser::PREVIEW_WIDTH = 310.0f;
const float FourWins::Menu::AvatarChooser::PREVIEW_HEIGHT = 469.0f;

FourWins::Menu::AvatarChooser::AvatarChooser(sf::RenderWindow &window, unsigned int choiceCount) :
	window(&window),
	bb(new sf::RectangleShape()),
	previewShape(new sf::RectangleShape(sf::Vector2f(AvatarChooser::PREVIEW_WIDTH, AvatarChooser::PREVIEW_HEIGHT))),
	previewTextShape(new sf::RectangleShape(sf::Vector2f(AvatarChooser::PREVIEW_WIDTH, 40.0f))),
	previewText(new sf::Text()),
	choices(new AvatarChoice[choiceCount]),
	CHOICE_COUNT(choiceCount),
	selectedChoice(choices),
	selectionChanged(false)
{
	for (unsigned int i = 0; i < choiceCount; ++i)
	{
		this->choices[i].thumbnailShape = new sf::RectangleShape(sf::Vector2f(AvatarChooser::THUMB_WIDTH, AvatarChooser::THUMB_HEIGHT));
		this->choices[i].previewString = new sf::String();
		this->choices[i].previewTextureRect = new sf::IntRect();
		this->choices[i].thumbnailShape->setOutlineColor(sf::Color::Transparent);
		this->choices[i].thumbnailShape->setOutlineThickness(1.6f);
	}
}

FourWins::Menu::AvatarChooser::~AvatarChooser()
{
	for (unsigned int i = 0; i < this->CHOICE_COUNT; ++i)
	{
		delete this->choices[i].thumbnailShape;
		delete this->choices[i].previewString;
	}
	delete[] this->choices;
	delete this->bb;
	delete this->previewShape;
	delete this->previewTextShape;
	delete this->previewText;
}

void FourWins::Menu::AvatarChooser::init()
{
	const float rowCount = (this->CHOICE_COUNT % 2u == 0) ? 2.0f : 3.0f;
	const float columnCount = static_cast<float>(this->CHOICE_COUNT / 2u);

	float bbWidth = columnCount * AvatarChooser::THUMB_WIDTH + (columnCount - 1) * AvatarChooser::THUMB_SPACE;
	float bbHeight = rowCount * AvatarChooser::THUMB_HEIGHT + (rowCount - 1) * AvatarChooser::THUMB_SPACE;
	this->bb->setSize(sf::Vector2f(bbWidth, bbHeight));
	this->bb->setFillColor(sf::Color(0, 0, 0, 100));
	this->bb->setOutlineColor(sf::Color::Black);
	this->bb->setOutlineThickness(2.0f);

	this->previewTextShape->setFillColor(sf::Color(0, 0, 0, 175));
	this->previewText->setCharacterSize(32u);
	this->previewText->setColor(Menu::TEXT_COLOR);
}

Menu::AvatarChooser &FourWins::Menu::AvatarChooser::setTexture(const sf::Texture *texture)
{
	this->previewShape->setTexture(texture);

	for (unsigned int i = 0; i < this->CHOICE_COUNT; ++i)
	{
		this->choices[i].thumbnailShape->setTexture(texture);
	}
	return *this;
}

Menu::AvatarChooser &FourWins::Menu::AvatarChooser::setThumbnailBoxPosition(const sf::Vector2f &position)
{
	this->bb->setPosition(position);

	const unsigned int rowCount = (this->CHOICE_COUNT % 2u == 0) ? 2u : 3u;
	const unsigned int columnCount = this->CHOICE_COUNT / 2u;

	for (unsigned int i = 0; i < this->CHOICE_COUNT; ++i)
	{
		float thumbXPos = position.x + static_cast<float>(i % columnCount)* (AvatarChooser::THUMB_WIDTH + AvatarChooser::THUMB_SPACE);
		
		//first row
		float Ymultiplier = 0.0f;

		if (i >= columnCount)
		{
			//second row
			Ymultiplier = 1.0f;

			if (rowCount == 3u && i == (this->CHOICE_COUNT - 1u))
			{
				//third row
				Ymultiplier = 2.0f;
			}
		}
		float thumbYPos = position.y + Ymultiplier * (AvatarChooser::THUMB_HEIGHT + AvatarChooser::THUMB_SPACE);

		this->choices[i].thumbnailShape->setPosition(sf::Vector2f(thumbXPos, thumbYPos));
	}
	return *this;
}

Menu::AvatarChooser &FourWins::Menu::AvatarChooser::setPreviewPosition(const sf::Vector2f &position)
{
	const sf::Vector2f &previewShapeSize = this->previewShape->getSize();
	const sf::Vector2f &previewTextShapeSize = this->previewTextShape->getSize();

	this->previewShape->setPosition(position);
	this->previewTextShape->setPosition(sf::Vector2f(position.x, position.y + previewShapeSize.y - previewTextShapeSize.y));
	this->previewText->setPosition(this->previewTextShape->getPosition());
	return *this;
}

Menu::AvatarChooser &FourWins::Menu::AvatarChooser::setTextureRectsForEntry(unsigned int index, const sf::IntRect &thumbRect, const sf::IntRect &previewRect)
{
	if (index < this->CHOICE_COUNT)
	{
		this->choices[index].thumbnailShape->setTextureRect(thumbRect);
		this->choices[index].previewTextureRect->left = previewRect.left;
		this->choices[index].previewTextureRect->top = previewRect.top;
		this->choices[index].previewTextureRect->width = previewRect.width;
		this->choices[index].previewTextureRect->height = previewRect.height;
	}
	return *this;
}

Menu::AvatarChooser &FourWins::Menu::AvatarChooser::setEnumForEntry(unsigned int index, Menu::Avatars value)
{
	if (index < this->CHOICE_COUNT)
	{
		this->choices[index].avatar = value;
	}
	return *this;
}

Menu::AvatarChooser &FourWins::Menu::AvatarChooser::setStringForEntry(unsigned int index, const sf::String &str)
{
	if (index < this->CHOICE_COUNT)
	{
		(*this->choices[index].previewString) = str;
	}
	return *this;
}

Menu::AvatarChooser &FourWins::Menu::AvatarChooser::setDefaultEntry(unsigned int index)
{
	if (index < this->CHOICE_COUNT)
	{
		this->selectedChoice = &this->choices[index];
		this->previewShape->setTextureRect(*this->choices[index].previewTextureRect);
		this->previewText->setString(*this->choices[index].previewString);

		float previewTextX = this->previewTextShape->getPosition().x +
			(this->previewTextShape->getSize().x / 2.0f) -
			(this->previewText->getLocalBounds().width / 2.0f);

		this->previewText->setPosition(sf::Vector2f(previewTextX, this->previewText->getPosition().y));
	}
	return *this;
}

void FourWins::Menu::AvatarChooser::update(const sf::Event &e, const sf::Vector2f &mousePosition)
{
	resetHover();

	if (this->bb->getGlobalBounds().contains(mousePosition))
	{
		switch (e.type)
		{
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseMoved:
			checkForHover(mousePosition);
			break;
		case sf::Event::MouseButtonReleased:
			checkForSelected(mousePosition);
			resetHover();
			break;
		}
	}
}

void FourWins::Menu::AvatarChooser::draw()
{
	this->window->draw(*this->bb);

	for (unsigned int i = 0; i < this->CHOICE_COUNT; ++i)
	{
		this->window->draw(*this->choices[i].thumbnailShape);
	}

	this->window->draw(*this->previewTextShape);
	this->window->draw(*this->previewText);
}

void FourWins::Menu::AvatarChooser::resetHover()
{
	for (unsigned int i = 0; i < this->CHOICE_COUNT; ++i)
	{
		if (&this->choices[i] == this->selectedChoice)
		{
			this->choices[i].thumbnailShape->setOutlineColor(sf::Color::Red);
		}
		else
		{
			this->choices[i].thumbnailShape->setOutlineColor(sf::Color::Transparent);
		}
	}

	this->previewShape->setTextureRect(*this->selectedChoice->previewTextureRect);
	this->previewText->setString(*this->selectedChoice->previewString);

	float previewTextX = this->previewTextShape->getPosition().x +
		(this->previewTextShape->getSize().x / 2.0f) -
		(this->previewText->getLocalBounds().width / 2.0f);

	this->previewText->setPosition(sf::Vector2f(previewTextX, this->previewText->getPosition().y));
}

void FourWins::Menu::AvatarChooser::checkForHover(const sf::Vector2f &mousePosition)
{
	for (unsigned int i = 0; i < this->CHOICE_COUNT; ++i)
	{
		if (this->choices[i].thumbnailShape->getGlobalBounds().contains(mousePosition))
		{
			if (&this->choices[i] != this->selectedChoice)
			{
				this->choices[i].thumbnailShape->setOutlineColor(sf::Color::Yellow);
			}
			this->previewShape->setTextureRect(*this->choices[i].previewTextureRect);
			this->previewText->setString(*this->choices[i].previewString);

			float previewTextX = this->previewTextShape->getPosition().x +
				(this->previewTextShape->getSize().x / 2.0f) -
				(this->previewText->getLocalBounds().width / 2.0f);

			this->previewText->setPosition(sf::Vector2f(previewTextX, this->previewText->getPosition().y));
			break;
		}
	}
}

void FourWins::Menu::AvatarChooser::checkForSelected(const sf::Vector2f &mousePosition)
{
	for (unsigned int i = 0; i < this->CHOICE_COUNT; ++i)
	{
		if (&this->choices[i] != this->selectedChoice && this->choices[i].thumbnailShape->getGlobalBounds().contains(mousePosition))
		{
			this->selectedChoice = &this->choices[i];
			this->selectionChanged = true;
		}
	}
}