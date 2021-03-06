#pragma once
#include <stdint.h>

#include "Player.h"


struct AiOptions{
    bool useIntelligentMeepleChoosing;
    bool useIntelligentMeeplePositioning;
};


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
    bool threadedSimulator;                 //true: the threadedSimulator should be used
    
    PlayerType playerType[2];
    ResourceManager::ResourceRect avatar[2];

    bool fast;                              //The AI doesn't perform a sleep before it's tasks.
    bool noAIsim;                           //Immediate.The AI's meeples are not slowly moved to the board. They will be positioned immediately.
    bool musicMuted; 
	bool effectsMuted;


    //These options are passed to the AI's (if neccessary):
    AiOptions aiOptions[2];

    GameSettings();
};

Player* createPlayer(const GameSettings& settings, uint8_t playerNum);  //Creates a new player with the set options; needs to be deleted()
I_Player* createI_Player(const GameSettings& settings, uint8_t playerNum); //Creates a new I_Player with the set options; needs to be deleted()