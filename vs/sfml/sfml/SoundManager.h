#pragma once
#include <SFML/Audio/Sound.hpp>
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

public:	
    enum Music{
        BACKGROUND,// BACKGROUND2, BACKGROUND3,
        SHEEP, MEEPLE_DROP, MEEPLE_WIN_DROP, GAME_START, SELECT, OH_YEAH
    };

    SoundManager();
	void loadResourcesFromFileSys();
	sf::Music* getMusic(Music type);
};



