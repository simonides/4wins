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
	sf::Texture soundSprite;
	sf::Texture tutorialBtnSprite;

	sf::Texture menuAtlas;

	sf::Texture splashscreen;

	sf::Texture tutorial[9];

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
		MENU_ATLAS,
		TEXT_SPRITE,
		SOUND_SPRITE,
		TUTORIAL_BTN_SRITE,

		SPLASH_SCREEN, TUT1, TUT2, TUT3, TUT4, TUT5, TUT6, TUT7, TUT8, TUT9

	};

	enum ResourceRect
	{
		BACKGROUND, BACKGROUND_WINDOW,
		PROFESSOR_JENKINS, SMOOTH_STEVE, HIPSTER_HENRY, BOYBAND_BILLY,
		BOOKWORM_BETTY, FASHION_FABIENNE, HIPPIE_HILDY, SMOKIN_STACY,

		MENU_HEADLINE, MENU_STARTBTN, MENU_STARTBTN_H, MENU_FRAME_UP, MENU_FRAME_DOWN,
		TUT_BTN, TUT_BTN_HIGHLIGHT,

		TEXT_PLAYER_1, TEXT_PLAYER_2,
		TEXT_CHOOSE, TEXT_SELECT, TEXT_A_MEEPLE, TEXT_A_POSITION,
		TEXT_WINS, TEXT_TIE, ARROW_UP, TEXT_PAUSE, TEXT_SKIP_TO_MENU,

		SND_MUSIC_MUTE, SND_MUSIC_PLAY, SND_EFFECTS_MUTE, SND_EFFECTS_PLAY,

		FULL_SCREEN_RECT,
	};
	ResourceManager();

	sf::Texture* getTexture(ResourceTexture textureType);
	sf::IntRect ResourceManager::getTextureRect(ResourceRect rectType) const;
	sf::Font* getFont(ResourceFont fontType);
};

