#pragma once
#include <stdint.h>

#include "Player.h"

class GameSettings{
    enum PlayerType
    {
        HUMAN,
        STUPID_AI,
        RANDOM_AI,
        THINKING_AI,
        SMART_AI
    };

    PlayerType playerType[2];
    ResourceManager::ResourceRect avatar[2];

    bool fast;
    bool noAIsim;
    bool muted;

    bool option1;
    bool option2;
public:
    GameSettings();
    void setPlayerType(uint8_t playerNum, PlayerType player);
    PlayerType getPlayerType(uint8_t playerNum) const;

    void setOptions(bool fast, bool noAIsim, bool muted);
    void setAvatar(uint8_t playerNum, ResourceManager::ResourceRect avatar);
    void setPlayerOptions(bool option1, bool option2);


    Player* createPlayer(uint8_t playerNum) const;  //Creates a new player with the set options; needs to be deleted()
};

