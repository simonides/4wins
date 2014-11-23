#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

// for backgroundmusic .. streams from filesys
enum MusicType
{
	BACKGROUND1, BACKGROUND2, BACKGROUND3
};
//short clips with fast responsetime, is kept in memory
enum SoundType
{
	SHEEP
};

class Sound
{
private:
	sf::Sound sound1;

	sf::Music backgroundMusic1;
	sf::Music backgroundMusic2;
	sf::Music backgroundMusic3;


public:
	
	Sound();
	~Sound();
	void loadResourcesFromFileSys();
	sf::Music* getMusic(MusicType type);
	sf::Sound* getSound(SoundType type);
};



