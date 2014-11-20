#include "ThreadedGameSimulator.h"
#include "Meeple.h"
#include "MeepleBag.h"
#include "Board.h"
#include "I_Player.h"
#include "config.h"

#include <assert.h>
#include <string>
#include <iostream>





std::string ThreadCommand::toString(){
    return type == TERMINATE ? "terminate" : type == SELECT_OPPONENTS_MEEPLE ? "select opponent's meeple" : type == SELECT_MEEPLE_POSITION ? "select meeple position" : "reset player";
}



ThreadedGameSimulator::ThreadedGameSimulator(I_Player& player1, I_Player& player2) : GameSimulator(player1, player2),
                                                                                     controller1(player1), 
                                                                                     controller2(player2) {    
}



void ThreadedGameSimulator::reset(){
    bag[0]->reset();
    bag[1]->reset();
    board->reset();
    controller1.run_resetPlayer();
    controller2.run_resetPlayer();
    controller1.confirmTaskCompletion();
    controller2.confirmTaskCompletion();
}





//a have round cycle, where a player chooses a meeple, and the other player sets it
#pragma warning(once: 4100)
void ThreadedGameSimulator::runGameCycle(I_Player* player, I_Player* opponent, const GameState& gameStateForPlayer, const GameState& gameStateForOpponent, int playerNr){

    ThreadController *playerController, *opponentController;
    if (player == controller1.getAssociatedPlayer()){
        playerController = &controller1;
        opponentController = &controller2;
    }else{
        playerController = &controller2;
        opponentController = &controller1;
    }


    playerController->run_selectOpponentsMeeple(gameStateForPlayer);            //player selects a meeple
    //while (!playerController->isResultAvailable()){ std::cout << '.'; }
    const Meeple& toSet = playerController->getOpponentsMeeple();
    Meeple* meeple = bag[(playerNr + 1) % 2]->removeMeeple(toSet);              //remove meeple from opponent's bag          
    

    opponentController->run_selectMeeplePosition(gameStateForOpponent, *meeple); //select a position
    //while (!opponentController->isResultAvailable()){ std::cout << '.'; }
    BoardPos pos = opponentController->getMeeplePosition();
    assert(pos.x < 4 && pos.y < 4);
    board->setMeeple(pos, *meeple);                                             //set the meeple
    
    //Debug:
    #if PRINT_BOARD_TO_CONSOLE
        std::cout << "Player " << playerNr + 1 << " chose meeple \"" << toSet.toString() << '\"' << std::endl;
        std::cout << "Player " << (playerNr + 1) % 2 + 1 << " sets meeple to " << pos.toString() << std::endl;
        board->print(std::cout);
    #endif
    #if STEP_BY_STEP
        std::cin.ignore();
    #endif
}