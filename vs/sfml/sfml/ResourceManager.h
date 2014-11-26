#pragma once
#include <SFML/Graphics.hpp>

class ResourceManager
{
private:
	sf::Font roboto;

	sf::Texture meepleSprite;
	sf::Texture glowSprite;

	sf::Texture boardTexture;
	sf::Texture fieldTexture;

	sf::Texture backgroundSprite;

	sf::Texture exitButtonTexture;
	sf::Texture reloadButtonTexture;
	sf::Texture menuButtonTexture;

	sf::Texture particleSprite;
	sf::Texture avatarSprite;

	sf::Texture menuAtlas;

	void loadResourcesFromFileSys(bool setSmooth); // invoked by the constructor

public:
	enum ResourceFont
	{
		ROBOTO 
	};
	enum ResourceTexture
	{
		MEEPLE_SPRITE, MEEPLE_GLOW_SPRITE,
		BOARD_TEX,
		FIELD_TEX,
		BACKGROUND_TEX,
		EXIT_BTN_TEX, RELOAD_BTN_TEX, MENU_BTN_TEX,
		PARTICLE_SPRITE,
		AVATAR_SPRITE,
		MENU_ATLAS
	};

	enum ResourceRect
	{
		BACKGROUND, BACKGROUND_WINDOW,
		MR_PROPER, UNCLE_BENS, RONALD, SMILEY, ELLE,
		MENU_HEADLINE, MENU_STARTBTN, MENU_STARTBTN_H, MENU_FRAME_UP, MENU_FRAME_DOWN
	};
	ResourceManager();

	sf::Texture* getTexture(ResourceTexture textureType);
	sf::IntRect ResourceManager::getTextureRect(ResourceRect rectType) const;
	sf::Font* getFont(ResourceFont fontType);
};

