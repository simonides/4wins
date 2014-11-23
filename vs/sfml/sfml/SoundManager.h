#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>


class SoundManager{
private:
	sf::Sound sound1;

	sf::Music backgroundMusic1;
	sf::Music backgroundMusic2;
	sf::Music backgroundMusic3;

public:	
    enum Music{
        BACKGROUND1, BACKGROUND2, BACKGROUND3
    };
    enum Sound{
        SHEEP
    };

    SoundManager();
	void loadResourcesFromFileSys();
	sf::Music* getMusic(Music type);
    sf::Sound* getSound(Sound type);
};



