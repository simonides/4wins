#include "GameSimulator.h"
#include "Meeple.h"
#include "MeepleBag.h"
#include "Board.h"
#include "I_Player.h"
#include "config.h"

#include <assert.h>
#include <string>
#include <iostream>


GameSimulator::GameSimulator(I_Player& player1, I_Player& player2) : board(new Board()){
    assert(&player1 != &player2);   //hehehe, never try to crash my game
    player[0] = &player1;
    player[1] = &player2;
    bag[0] = new MeepleBag(MeepleColor::WHITE);
    bag[1] = new MeepleBag(MeepleColor::BLACK);
    gameStates[0] = new GameState(bag[0], bag[1], board);
    gameStates[1] = new GameState(bag[1], bag[0], board);
} 


GameSimulator::~GameSimulator(){
    delete board;
    delete gameStates[1];
    delete gameStates[0];
    delete bag[1];
    delete bag[0];
}


void GameSimulator::reset(){
    bag[0]->reset();
    bag[1]->reset();
    board->reset();
    player[0]->reset();
    player[1]->reset();
}


GameWinner::Enum GameSimulator::runManyGames(unsigned int cycles, bool printState){
    unsigned int pw1 = 0, pw2 = 0, ties = 0;
   
    if (printState){
        std::cout << "Calculating..." << std::endl;
    }
    for (unsigned int g = 0; g<cycles; ++g){
        GameWinner::Enum winner = runGame();
        switch (winner){
            case GameWinner::PLAYER_1: pw1++; break;
            case GameWinner::PLAYER_2: pw2++; break;
            case GameWinner::TIE: ties++;  break;
        }
        reset();
        if (printState && g % 100 == 0){
            std::cout << "Player 1 won " << pw1 << " times, and Player 2 won " << pw2 << " times. There were " << ties << " Ties.\r";
        }
    }
    if (printState){
        std::cout << "Player 1 won " << pw1 << " times, and Player 2 won " << pw2 << " times. There were " << ties << " Ties." << std::endl;
    }
    if (pw1 == pw2){
        return GameWinner::TIE;
    }
    if (pw1 > pw2){
        return GameWinner::PLAYER_1;
    }
    return GameWinner::PLAYER_2;    
}

//Game Loop for one game, until there is a winner or the board is full
GameWinner::Enum GameSimulator::runGame(){
    for (;;){
        runGameCycle(0);
        if (board->checkWinSituation()){    //player2 won
            #if PRINT_WINNER_PER_ROUND
                std::cout << "Player 2 wins!" << std::endl;
            #endif
            return GameWinner::PLAYER_2;
        }

        runGameCycle(1);
        if (board->checkWinSituation()){    //player1 won
            #if PRINT_WINNER_PER_ROUND
                std::cout << "Player 1 wins!" << std::endl;
            #endif
            return GameWinner::PLAYER_1;
        }

        if (board->isFull()){
            #if PRINT_WINNER_PER_ROUND
                std::cout << "Tie! There is no winner." << std::endl;
            #endif
            return GameWinner::TIE;
        }
    }
}


//a have round cycle, where a player chooses a meeple, and the other player sets it
void GameSimulator::runGameCycle(uint8_t playerNr){
    uint8_t opponentNr = (playerNr + 1) % 2;
    const Meeple& toSet = player[playerNr]->selectOpponentsMeeple(*gameStates[playerNr]);       //player selects a meeple
    Meeple* meeple = bag[(playerNr + 1) % 2]->removeMeeple(toSet);              //remove meeple from opponent's bag          
    
    BoardPos pos = player[opponentNr]->selectMeeplePosition(*gameStates[opponentNr], *meeple);  //select a position
    assert(pos.x < 4 && pos.y < 4);
    board->setMeeple(pos, *meeple);                                             //set the meeple
    
    //Debug:
    #if PRINT_BOARD_TO_CONSOLE
        std::cout << "Player " << playerNr + 1 << " chose meeple \"" << toSet.toString() << '\"' << std::endl;
        std::cout << "Player " << opponentNr + 1 << " sets meeple to " << pos.toString() << std::endl;
        board->print(std::cout);
    #endif
    #if STEP_BY_STEP
        std::cin.ignore();
    #endif
}