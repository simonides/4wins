#include "Game.h"
#include "Meeple.h"
#include "MeepleBag.h"
#include "BoardState.h"
#include "I_Player.h"


#include <iostream>



Game::Game(I_Player& player1, I_Player& player2) : player1(player1), player2(player2){
    bag[0] = new MeepleBag(MeepleColor::WHITE);
    bag[1] = new MeepleBag(MeepleColor::BLACK);
    board = new BoardState();
}


Game::~Game(){
    delete bag[0];
    delete bag[1];
    delete board;
}


GameWinner::Enum Game::runGame(){
    std::cout << "Running the game..." << std::endl;

    for (;;){
        const Meeple& toSet1 = player1.selectOpponentsMeeple(*bag[0], *bag[1], *board);
        BoardPos pos = player2.selectMeeplePosition(*bag[0], *bag[1], *board, toSet1);
        //todo: set meeple here
        //todo: check win-condition here
        if (true /*board full*/){
            return GameWinner::TIE;
        }

        const Meeple& toSet2 = player2.selectOpponentsMeeple(*bag[1], *bag[0], *board);
        pos = player2.selectMeeplePosition(*bag[1], *bag[0], *board, toSet2);
        //todo: set meeple here
        //todo: check win-condition here
        if (true /*board full*/){
            return GameWinner::TIE;
        }
    }

}