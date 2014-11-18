#pragma once

#include "GameSimulator.h"
#include "ThreadController.h"

class MeepleBag;
class Board;
class I_Player;



//Contains all information about a game, and handles the game loop
//A Game-object is usable for exacly one round
class ThreadedGameSimulator : public GameSimulator
{
private:
    ThreadController controller1;
    ThreadController controller2;

    void runGameCycle(I_Player* player, I_Player* opponent, GameState& gameStateForPlayer, GameState& gameStateForOpponent, int playerNr);
public:
    ThreadedGameSimulator(I_Player& player1, I_Player& player2); //Initialises the game with 2 players
    void reset();                               //Reinitialises the object    
};

