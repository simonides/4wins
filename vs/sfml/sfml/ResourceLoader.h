#pragma once
#include <SFML/Graphics.hpp>
class ResourceLoader
{
private:
	sf::Font roboto;

	sf::Texture meepleSprite;
	sf::Texture glowSprite;

	sf::Texture boardTexture;
	sf::Texture fieldTexture;

	sf::Texture backgroundTexture;

	sf::Texture exitButtonTexture;
	sf::Texture reloadButtonTexture;
	sf::Texture menuButtonTexture;

	sf::Texture particleSprite;

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
		PARTICLE_SPRITE
	};
	ResourceLoader();
	~ResourceLoader();

	sf::Texture* getTexture(ResourceTexture textureType);
	sf::Font* getFont(ResourceFont fontType);

};

