#include "StupidAI.h"

#include <assert.h>

#include "Board.h"
#include "MeepleBag.h"
#include "GameState.h"

#pragma warning( disable: 4100 )


const Meeple& StupidAI::selectOpponentsMeeple(const GameState& gameState){
    return *(gameState.opponentBag->getMeeple(0));
}

BoardPos StupidAI::selectMeeplePosition(const GameState& gameState, const Meeple& meepleToSet){
    for (uint8_t y = 0; y < 4; ++y){
        for (uint8_t x = 0; x < 4; ++x){
            if (gameState.board->isFieldEmpty({ x, y })){
                return{ x, y };
            }
        }
    }
    assert(false);  //no empty board field avaiable
    return{ 42, 42 };
}