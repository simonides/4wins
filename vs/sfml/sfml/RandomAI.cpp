#include "RandomAI.h"

#include <assert.h>
#include <algorithm>
#include <time.h>  

#include "Game.h"
#include "Board.h"
#include "MeepleBag.h"

#pragma warning( disable: 4100 )


RandomAI::RandomAI(){
    srand(static_cast<unsigned int>(time(NULL)));
}

const Meeple& RandomAI::selectOpponentsMeeple(const GameState& gameState){
    return *(gameState.opponentBag->getMeeple(rand() % gameState.opponentBag->getMeepleCount()));
}

BoardPos RandomAI::selectMeeplePosition(const GameState& gameState, const Meeple& meepleToSet){
    return gameState.board->getRandomEmptyField();
}
