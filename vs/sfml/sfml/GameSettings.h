#pragma once
#include <stdint.h>

#include "Player.h"




struct GameSettings{
    enum PlayerType
    {
        HUMAN,
        STUPID_AI,
        RANDOM_AI,
        THINKING_AI,
        SMART_AI
    };

    unsigned int simulator;                 //>0: use the simulator instead of the graphical output. Numer = number of games to simulate
    bool threadedSimulator;


    PlayerType playerType[2];
    ResourceManager::ResourceRect avatar[2];

    bool fast;
    bool noAIsim;
    bool muted;

    bool option1;
    bool option2;

    GameSettings();
};

Player* createPlayer(const GameSettings& settings, uint8_t playerNum);  //Creates a new player with the set options; needs to be deleted()
I_Player* createI_Player(const GameSettings& settings, uint8_t playerNum); //Creates a new I_Player with the set options; needs to be deleted()