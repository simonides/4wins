#include "ResourceLoader.h"
#include "config.h"
#include <assert.h>
#include <iostream>

ResourceLoader::ResourceLoader()
{
	loadResourcesFromFileSys(true);
}


ResourceLoader::~ResourceLoader()
{
}

void ResourceLoader::loadResourcesFromFileSys(bool setSmooth)
{
	if (!roboto.loadFromFile(WORKING_DIR + "Fonts\\roboto\\roboto-black.ttf")){
		std::cerr << "Couldn't load the Font: roboto-black.ttf" << std::endl;
		assert(false);
	}

	if (!meepleSprite.loadFromFile(WORKING_DIR + "pencilStyle.png")){
		std::cerr << "Couldn't load the texture: meepleSprites" << std::endl;
		assert(false);
	}
	meepleSprite.setSmooth(setSmooth);

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


	if (!backgroundTexture.loadFromFile(WORKING_DIR + "background.png")){
		std::cerr << "Couldn't load the texture: background" << std::endl;
		assert(false);
	}
	backgroundTexture.setSmooth(setSmooth);

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
	
}



sf::Texture* ResourceLoader::getTexture(ResourceTexture textureType)
{
	switch (textureType)
	{
	case MEEPLE_SPRITE: return &meepleSprite;
	case MEEPLE_GLOW_SPRITE: return &glowSprite;
	case BOARD_TEX: return &boardTexture;
	case FIELD_TEX: return &fieldTexture;
	case BACKGROUND_TEX: return &backgroundTexture;
	case EXIT_BTN_TEX: return &exitButtonTexture;
	case RELOAD_BTN_TEX: return &reloadButtonTexture;
	case MENU_BTN_TEX: return &menuButtonTexture;
	case PARTICLE_SPRITE: return &particleSprite;
	default: return &exitButtonTexture;
		//assert(false);
	}
}
sf::Font* ResourceLoader::getFont(ResourceFont fontType)
{
	switch (fontType)
	{
	case ROBOTO: return &roboto;
	default: return &roboto;
		//assert(false);
	}
}
