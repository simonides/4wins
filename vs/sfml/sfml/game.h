#pragma once

class MeepleBag;
class BoardState;
class I_Player;



struct GameWinner{
    enum Enum{
        PLAYER_1,
        PLAYER_2,
        TIE
    };
};



//Contains all information about a game, and handles the game loop
//A Game-object is usable for exacly one round
class Game
{
private:
    MeepleBag* bag[2];
    BoardState* board;

    I_Player& player1;
    I_Player& player2;

    void runGameCycle(I_Player& player, I_Player& opponent, int playerNr);
public:
    Game(I_Player& player1, I_Player& player2); //Initialises the game with 2 players
    void reset();                               //Reinitialises the object

    GameWinner::Enum runGame();                 //Runs the game, until it is over; returns the winner
    virtual ~Game();
};

