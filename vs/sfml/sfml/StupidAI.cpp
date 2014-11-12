#include "StupidAI.h"

#include <assert.h>

#include "Game.h"
#include "Board.h"
#include "MeepleBag.h"



const Meeple& StupidAI::selectOpponentsMeeple(const GameState& gameState){
    return *(gameState.opponentBag.getMeeple(0));
}

BoardPos StupidAI::selectMeeplePosition(const GameState& gameState, const Meeple& meepleToSet){
    for (unsigned int y = 0; y < 4; ++y){
        for (unsigned int x = 0; x < 4; ++x){
            if (gameState.board.isFieldEmpty({ x, y })){
                return{ x, y };
            }
        }
    }
    assert(false);  //no empty board field avaiable
    return{ 42, 42 };
}