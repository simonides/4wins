#include "StupidAI.h"


#include <assert.h>
#include "BoardState.h"
#include "MeepleBag.h"



const Meeple& StupidAI::selectOpponentsMeeple(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board){
    return **(opponentBag.getMeeples());
}

BoardPos StupidAI::selectMeeplePosition(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board, const Meeple& meepleToSet){
    for (unsigned int y = 0; y < 4; y++){
        for (unsigned int x = 0; x < 4; x++){
            if (board.isFieldEmpty({ x, y })){
                return{ x, y };
            }
        }
    }
    assert(false);  //no empty board field avaiable
    return{ 42, 42 };
}