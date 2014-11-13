#pragma once

#include "Game.h"
class MeepleBag;
class Board;
class I_Player;



//Contains all information about a game, and handles the game loop
//A Game-object is usable for exacly one round
class GameSimulator
{
private:
    MeepleBag* bag[2];
    Board* board;

    GameState* gameStatePlayer1;                //stores the gamestate for player 1 (buffered)
    GameState* gameStatePlayer2;

    I_Player* player1;
    I_Player* player2;

    void runGameCycle(I_Player* player, I_Player* opponent, GameState& gameStateForPlayer, GameState& gameStateForOpponent, int playerNr);
public:
    GameSimulator(I_Player& player1, I_Player& player2); //Initialises the game with 2 players
    void reset();                               //Reinitialises the object

    GameWinner::Enum runGame();                 //Runs the game, until it is over; returns the winner
    GameWinner::Enum runManyGames(unsigned int cycles, bool printState);        //Runs many game cycles (prints the winners, if printState is true), and returns which player won the most time

    virtual ~GameSimulator();
};

