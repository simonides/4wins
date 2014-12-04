#include "SoundManager.h"
#include <iostream>
#include <stdint.h>

#include "config.h"
#include "assert.h"


SoundManager::SoundManager() :
VOLUME_BACKGROUND(60.f),
VOLUME_EFFECTS(100.f)
{
	loadResourcesFromFileSys();
}

void SoundManager::loadResourcesFromFileSys(){
    if (!backgroundMusic.openFromFile(WORKING_DIR + "sounds/backgroundCancelingtheApocalypse.ogg")){
        std::cerr << "Couldn't load the music file: backgroundCancelingtheApocalypse" << std::endl;
        assert(false);
    }
    backgroundMusic.setLoop(true);
   
    if (!winMusic.openFromFile(WORKING_DIR + "sounds/winMusic.ogg")){
        std::cerr << "Couldn't load the music file: winMusic" << std::endl;
        assert(false);
    }
    winMusic.setLoop(true);
 
    if (!sheep.openFromFile(WORKING_DIR + "sounds/sheep.ogg")){
        std::cerr << "Couldn't load the sound file: sheep" << std::endl;
        assert(false);
    }
   
    if (!meepleDrop.openFromFile(WORKING_DIR + "sounds/meepleDrop.ogg")){
        std::cerr << "Couldn't load the sound file: meepleDrop" << std::endl;
        assert(false);
    }
    
    if (!meepleWinDrop.openFromFile(WORKING_DIR + "sounds/meepleWinDrop.ogg")){
        std::cerr << "Couldn't load the sound file: meepleWinDrop" << std::endl;
        assert(false);
    }
    
    if (!gameStart.openFromFile(WORKING_DIR + "sounds/gameStart.ogg")){
        std::cerr << "Couldn't load the sound file: gameStart" << std::endl;
        assert(false);
    }
    
    if (!select.openFromFile(WORKING_DIR + "sounds/select.ogg")){
        std::cerr << "Couldn't load the sound file: select" << std::endl;
        assert(false);
    }
    
    for (uint8_t i = 0; i < 5; ++i){
        if (!ohYeah[i].openFromFile(WORKING_DIR + "sounds/ohYeah" + static_cast<char>(i+'1') + ".ogg")){
            std::cerr << "Couldn't load the sound file: ohYeah" << static_cast<unsigned int>(i) << std::endl;
            assert(false);
        }
    }
	// set Volume
	setEffectsVolume(VOLUME_EFFECTS);
	setMusicVolume(VOLUME_BACKGROUND);
}

sf::Music* SoundManager::getMusic(Music type){
	switch (type){
	    case BACKGROUND:	    return &backgroundMusic;
        case WIN_MUSIC:         return &winMusic;

        case SHEEP:             return &sheep;
        case MEEPLE_DROP:       return &meepleDrop;
        case MEEPLE_WIN_DROP:   return &meepleWinDrop;
        case GAME_START:        return &gameStart;
        case SELECT:            return &select;
        case OH_YEAH:           return &ohYeah[ rand()%5 ];

        default: assert(false); return &sheep;
	}
}

void SoundManager::setEffectsVolume(float volume)
{
	for (uint8_t i = 0; i < 5; ++i){
		ohYeah[i].setVolume(volume);
	}
	gameStart.setVolume(volume);
	select.setVolume(volume);
	meepleWinDrop.setVolume(volume);
	meepleDrop.setVolume(volume);
	sheep.setVolume(volume);
	winMusic.setVolume(volume);
}

void SoundManager::setMusicVolume(float volume)
{
	backgroundMusic.setVolume(volume);
}
