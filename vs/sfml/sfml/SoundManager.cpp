#include "SoundManager.h"
#include <iostream>
#include <stdint.h>

#include "config.h"
#include "assert.h"


const float VOLUME_BACKGROUND = 50.f;
const float VOLUME_EFFECTS = 90.f;

SoundManager::SoundManager(){
	loadResourcesFromFileSys();
}



void SoundManager::loadResourcesFromFileSys(){
    if (!backgroundMusic.openFromFile(WORKING_DIR + "sounds/backgroundCancelingtheApocalypse.ogg")){
        std::cerr << "Couldn't load the music file: backgroundCancelingtheApocalypse" << std::endl;
        assert(false);
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(VOLUME_BACKGROUND);
    if (!winMusic.openFromFile(WORKING_DIR + "sounds/winMusic.ogg")){
        std::cerr << "Couldn't load the music file: winMusic" << std::endl;
        assert(false);
    }
    winMusic.setLoop(true);
    winMusic.setVolume(VOLUME_EFFECTS);

    if (!sheep.openFromFile(WORKING_DIR + "sounds/sheep.ogg")){
        std::cerr << "Couldn't load the sound file: sheep" << std::endl;
        assert(false);
    }
    sheep.setVolume(VOLUME_EFFECTS);
    if (!meepleDrop.openFromFile(WORKING_DIR + "sounds/meepleDrop.ogg")){
        std::cerr << "Couldn't load the sound file: meepleDrop" << std::endl;
        assert(false);
    }
    meepleDrop.setVolume(VOLUME_EFFECTS);
    if (!meepleWinDrop.openFromFile(WORKING_DIR + "sounds/meepleWinDrop.ogg")){
        std::cerr << "Couldn't load the sound file: meepleWinDrop" << std::endl;
        assert(false);
    }
    meepleWinDrop.setVolume(VOLUME_EFFECTS);
    if (!gameStart.openFromFile(WORKING_DIR + "sounds/gameStart.ogg")){
        std::cerr << "Couldn't load the sound file: gameStart" << std::endl;
        assert(false);
    }
    gameStart.setVolume(VOLUME_EFFECTS);
    if (!select.openFromFile(WORKING_DIR + "sounds/select.ogg")){
        std::cerr << "Couldn't load the sound file: select" << std::endl;
        assert(false);
    }
    select.setVolume(VOLUME_EFFECTS);
    for (uint8_t i = 0; i < 5; ++i){
        if (!ohYeah[i].openFromFile(WORKING_DIR + "sounds/ohYeah" + static_cast<char>(i+'1') + ".ogg")){
            std::cerr << "Couldn't load the sound file: ohYeah" << static_cast<unsigned int>(i) << std::endl;
            assert(false);
        }
        ohYeah[i].setVolume(VOLUME_EFFECTS);
    }
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

