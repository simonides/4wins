#include "SoundManager.h"
#include "config.h"
#include "assert.h"
#include <iostream>


const float VOLUME_BACKGROUND = 60.f;
SoundManager::SoundManager(){
	loadResourcesFromFileSys();
}



void SoundManager::loadResourcesFromFileSys(){
	if (!backgroundMusic1.openFromFile(WORKING_DIR + "Sounds\\backgroundCancelingtheApocalypse.ogg")){
		std::cerr << "Couldn't load the texture: replay" << std::endl;
		assert(false);
	}
	backgroundMusic1.setLoop(true);
	backgroundMusic1.setVolume(VOLUME_BACKGROUND);
}

sf::Music* SoundManager::getMusic(Music type){
	switch (type){
	    case BACKGROUND1:			return &backgroundMusic1;
	    case BACKGROUND2:			return &backgroundMusic1;		// not used by now
	    case BACKGROUND3:			return &backgroundMusic1;		// not used by now
        default: assert(false);     return &backgroundMusic1;
	}
}

sf::Sound* SoundManager::getSound(Sound type){
	switch (type){
	    case SHEEP:					return &sound1;					// not used by now
        default: assert(false);     return &sound1;
	}
}

