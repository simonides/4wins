#pragma once
#include <SFML/Audio/Music.hpp>


class SoundManager{
private:
    sf::Music sheep;
    sf::Music meepleDrop;
    sf::Music meepleWinDrop;
    sf::Music gameStart;
    sf::Music select;
    sf::Music ohYeah[5];

    sf::Music backgroundMusic;
    sf::Music winMusic;

public:	
    enum Music{
        BACKGROUND, WIN_MUSIC,
        SHEEP, MEEPLE_DROP, MEEPLE_WIN_DROP, GAME_START, SELECT, OH_YEAH
    };

    SoundManager();
	void loadResourcesFromFileSys();
	sf::Music* getMusic(Music type);
};



