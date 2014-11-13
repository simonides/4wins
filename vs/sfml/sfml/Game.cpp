#include "Game.h"
#include "Meeple.h"
#include "MeepleBag.h"
#include "Board.h"
#include "I_Player.h"
#include "config.h"

#include <assert.h>
#include <string>
#include <iostream>


using namespace std;

Game::Game(I_Player& player1, I_Player& player2) : player1(player1), player2(player2) {
    bag[0] = new MeepleBag(MeepleColor::WHITE);
    bag[1] = new MeepleBag(MeepleColor::BLACK);
    board = new Board();
    gameStatePlayer1 = new GameState(*bag[0], *bag[1], *board);
    gameStatePlayer2 = new GameState(*bag[1], *bag[0], *board);
}


Game::~Game(){
    delete gameStatePlayer2;
    delete gameStatePlayer1;
    delete board;
    delete bag[1];
    delete bag[0];
}


void Game::reset(){
    bag[0]->reset();
    bag[1]->reset();

    /*delete bag[1];
    delete bag[0];
    bag[0] = new MeepleBag(MeepleColor::WHITE);
    bag[1] = new MeepleBag(MeepleColor::BLACK);
    gameStatePlayer1 = new GameState(*bag[0], *bag[1], *board);
    gameStatePlayer2 = new GameState(*bag[1], *bag[0], *board);*/

    board->reset();
    player1.reset();
    player2.reset();
}



//Game Loop for one game, until there is a winner or the board is full
GameWinner::Enum Game::runGame(){
    for (;;){
        runGameCycle(player1, player2, *gameStatePlayer1, *gameStatePlayer2 ,0);
        if (board->checkWinSituation()){    //player2 won
            #if PRINT_WINNER_PER_ROUND
                cout << "Player 2 wins!" << endl;
            #endif
            return GameWinner::PLAYER_2;
        }

        runGameCycle(player2, player1, *gameStatePlayer2, *gameStatePlayer1, 1);
        if (board->checkWinSituation()){    //player1 won
            #if PRINT_WINNER_PER_ROUND
                cout << "Player 1 wins!" << endl;
            #endif
            return GameWinner::PLAYER_1;
        }

        if (board->isFull()){
            #if PRINT_WINNER_PER_ROUND
                cout << "Tie! There is no winner." << endl;
            #endif
            return GameWinner::TIE;
        }
    }
}


//a have round cycle, where a player chooses a meeple, and the other player sets it
void Game::runGameCycle(I_Player& player, I_Player& opponent, GameState& gameStateForPlayer, GameState& gameStateForOpponent, int playerNr){

    const Meeple& toSet = player.selectOpponentsMeeple(gameStateForPlayer);    //player selects a meeple
    Meeple* meeple = bag[(playerNr + 1) % 2]->removeMeeple(toSet);              //remove meeple from opponent's bag          
    
    BoardPos pos = opponent.selectMeeplePosition(gameStateForOpponent, *meeple); //select a position
    assert(pos.x < 4 && pos.y < 4);
    board->setMeeple(pos, *meeple);                                             //set the meeple
    
    //Debug:
    #if PRINT_BOARD_TO_CONSOLE
        cout << "Player " << playerNr + 1 << " chose meeple \"" << toSet.toString() << '\"' << endl;
        cout << "Player " << (playerNr + 1) % 2 + 1 << " sets meeple to (" << static_cast<int>(pos.x) << 'x' << static_cast<int>(pos.y) << ')' << endl;
        board->print(cout);
    #endif
    #if STEP_BY_STEP
        cin.ignore();
    #endif
}