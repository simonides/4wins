#include "ResourceManager.h"
#include "config.h"
#include <assert.h>
#include <iostream>

ResourceManager::ResourceManager(){
	loadResourcesFromFileSys(true);
}


void ResourceManager::loadResourcesFromFileSys(bool setSmooth)
{
	if (!roboto.loadFromFile(WORKING_DIR + "Fonts/roboto/roboto-black.ttf")){
		std::cerr << "Couldn't load the Font: roboto-black.ttf" << std::endl;
		assert(false);
	}

	if (!meepleSprite.loadFromFile(WORKING_DIR + "pencilStyle.png")){
		std::cerr << "Couldn't load the texture: meepleSprites" << std::endl;
		assert(false);
	}
	meepleSprite.setSmooth(setSmooth);

	if (!avatarSprite.loadFromFile(WORKING_DIR + "players.png")){
		std::cerr << "Couldn't load the texture: avatarSprite" << std::endl;
		assert(false);
	}
	avatarSprite.setSmooth(setSmooth);

	if (!glowSprite.loadFromFile(WORKING_DIR + "glow.png")){
		std::cerr << "Couldn't load the texture: glow" << std::endl;
		assert(false);
	}
	glowSprite.setSmooth(setSmooth);


	if (!boardTexture.loadFromFile(WORKING_DIR + "board.png")){
		std::cerr << "Couldn't load the texture: board" << std::endl;
		assert(false);
	}
	boardTexture.setSmooth(setSmooth);


	if (!fieldTexture.loadFromFile(WORKING_DIR + "field.png")){
		std::cerr << "Couldn't load the texture: field" << std::endl;
		assert(false);
	}
	fieldTexture.setSmooth(setSmooth);


	if (!backgroundSprite.loadFromFile(WORKING_DIR + "background.png")){
		std::cerr << "Couldn't load the texture: background" << std::endl;
		assert(false);
	}
	backgroundSprite.setSmooth(setSmooth);

	if (!exitButtonTexture.loadFromFile(WORKING_DIR + "quit.png")){
		std::cerr << "Couldn't load the texture: quit" << std::endl;
		assert(false);
	}
	exitButtonTexture.setSmooth(setSmooth);

	if (!reloadButtonTexture.loadFromFile(WORKING_DIR + "replay.png")){
		std::cerr << "Couldn't load the texture: replay" << std::endl;
		assert(false);
	}
	reloadButtonTexture.setSmooth(setSmooth);

	if (!particleSprite.loadFromFile(WORKING_DIR + "particles.png")){
		std::cerr << "Couldn't load the texture: particles" << std::endl;
		assert(false);
	}
	//particleSprite.setSmooth(true); //check performance first

	if (!menuButtonTexture.loadFromFile(WORKING_DIR + "menue.png")){
		std::cerr << "Couldn't load the texture: menue" << std::endl;
		assert(false);
	}
	menuButtonTexture.setSmooth(setSmooth);

	if (!menuAtlas.loadFromFile(WORKING_DIR + "menu_atlas.png")){
		std::cerr << "Couldn't load the texture: menu_atlas" << std::endl;
		assert(false);
	}
	menuAtlas.setSmooth(setSmooth);

	if (!textSprite.loadFromFile(WORKING_DIR + "textSheet.png")){
		std::cerr << "Couldn't load the texture: textSheet" << std::endl;
		assert(false);
	}
	textSprite.setSmooth(setSmooth);
}



sf::Texture* ResourceManager::getTexture(ResourceTexture textureType)
{
	switch (textureType)
	{
	case MEEPLE_SPRITE:			return &meepleSprite;
	case MEEPLE_GLOW_SPRITE:	return &glowSprite;
	case BOARD_TEX:				return &boardTexture;
	case FIELD_TEX:				return &fieldTexture;
	case BACKGROUND_TEX:		return &backgroundSprite;
	case EXIT_BTN_TEX:			return &exitButtonTexture;
	case RELOAD_BTN_TEX:		return &reloadButtonTexture;
	case MENU_BTN_TEX:			return &menuButtonTexture;
	case PARTICLE_SPRITE:		return &particleSprite;
	case AVATAR_SPRITE:			return &avatarSprite;
	case TEXT_SPRITE:			return &textSprite;
	case MENU_ATLAS:			return &menuAtlas;
	default:					return &exitButtonTexture;
	

		//assert(false);
	}
}

sf::IntRect ResourceManager::getTextureRect(ResourceRect rectType) const
{
	switch (rectType)
	{
	case SMOOTH_STEVE:			return sf::IntRect(0,     0, 128, 194);
	case PROFESSOR_JENKINS:		return sf::IntRect(128,   0, 128, 194);
	case HIPSTER_HENRY:			return sf::IntRect(256,   0, 128, 194);
	case BOYBAND_BILLY:			return sf::IntRect(384,   0, 128, 194);
	case BOOKWORM_BETTY:		return sf::IntRect(0,   194, 128, 194);
	case FASHION_FABIENNE:		return sf::IntRect(128, 194, 128, 194);
	case HIPPIE_HILDY:			return sf::IntRect(256, 194, 128, 194);
	case SMOKIN_STACY:			return sf::IntRect(384, 194, 128, 194);
	
	case BACKGROUND:			return sf::IntRect(0,	  0, 135,  68);
	case BACKGROUND_WINDOW:		return sf::IntRect(4,	 70,  31,  49);
	
	case TEXT_PLAYER_1:			return sf::IntRect(0,	  0, 762, 212);//
	case TEXT_PLAYER_2:			return sf::IntRect(0,	212, 762, 212);//

	case TEXT_WINS:				return sf::IntRect(0,   424, 581, 210);//
	case TEXT_TIE:				return sf::IntRect(582, 424, 441, 210);//

	case TEXT_CHOOSE:			return sf::IntRect(0,	635, 241, 73);//
	case TEXT_SELECT:			return sf::IntRect(0,	708, 203, 73);//
	case TEXT_A_MEEPLE:			return sf::IntRect(241, 635, 291, 73);//
	case TEXT_A_POSITION:		return sf::IntRect(203,	708, 324, 73);//

	case ARROW_UP:				return sf::IntRect(762,   0, 131, 243);//

	case MENU_HEADLINE:			return sf::IntRect(0,   0, 499, 140);
	case MENU_STARTBTN:			return sf::IntRect(500, 0, 350, 60);
	case MENU_STARTBTN_H:		return sf::IntRect(500, 64, 350, 60);
	case MENU_FRAME_UP:			return sf::IntRect(851, 0, 208, 30);
	case MENU_FRAME_DOWN:		return sf::IntRect(851, 32, 208, 30);

	default:					return sf::IntRect(0, 0, 16, 16);	//default something
	}
}
sf::Font* ResourceManager::getFont(ResourceFont fontType)
{
	switch (fontType)
	{
	case ROBOTO:				return &roboto;
	default: assert(false);		return &roboto;
		//
	}
}
