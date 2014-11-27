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
	sf::Texture textSprite;

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
		//text
		TEXT_SPRITE,

	};

	enum ResourceRect
	{
		BACKGROUND, BACKGROUND_WINDOW, 
		PROFESSOR_JENKINS, SMOOTH_STEVE, HIPSTER_HENRY, BOYBAND_BILLY, 
		BOOKWORM_BETTY, FASHION_FABIENNE, HIPPIE_HILDY,	SMOKIN_STACY,

		TEXT_PLAYER, TEXT_AI, TEXT_BLACK, TEXT_WHITE,
		TEXT_CHOOSE, TEXT_SELECT, TEXT_A, TEXT_MEEPLE, TEXT_POSITION,
		TEXT_WINS, TEXT_TIE
	};
	ResourceManager();

	sf::Texture* getTexture(ResourceTexture textureType);
	sf::IntRect ResourceManager::getTextureRect(ResourceRect rectType) const;
	sf::Font* getFont(ResourceFont fontType);
};

