#include "Sound.h"
#include "config.h"
#include "assert.h"
#include <iostream>


const float VOLUME_BACKGROUND = 60.f;
Sound::Sound()
{
	loadResourcesFromFileSys();
}


Sound::~Sound()
{
}

void Sound::loadResourcesFromFileSys()
{
	if (!backgroundMusic1.openFromFile(WORKING_DIR + "Sounds\\backgroundCancelingtheApocalypse.ogg")){
		std::cerr << "Couldn't load the texture: replay" << std::endl;
		assert(false);
	}
	backgroundMusic1.setLoop(true);
	backgroundMusic1.setVolume(VOLUME_BACKGROUND);
}

sf::Music* Sound::getMusic(MusicType type)
{
	switch (type)
	{
	case BACKGROUND1:			return &backgroundMusic1;
	case BACKGROUND2:			return &backgroundMusic1;		// not used by now
	case BACKGROUND3:			return &backgroundMusic1;		// not used by now
	default: break;

	}
}

sf::Sound* Sound::getSound(SoundType type)
{
	switch (type)
	{
	case SHEEP:					return &sound1;					// not used by now
	default: break;
	}
}

