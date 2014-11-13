#include "Player.h"


#include <assert.h>
#include "Board.h"
#include "MeepleBag.h"

#pragma warning( disable: 4100 )



const Meeple& Player::selectOpponentsMeeple(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Board& board){
    //TODO: the user has to use the mouse, in order to find the meeple
    return *opponentBag.getMeeple(0);
}

BoardPos Player::selectMeeplePosition(const MeepleBag& ownBag, const MeepleBag& opponentBag, const Board& board, const Meeple& meepleToSet){
    //TODO: the user has to use the mouse, in order to select a field    
    for (uint8_t y = 0; y < 4; ++y){
        for (uint8_t x = 0; x < 4; ++x){
            if (board.isFieldEmpty({ x, y })){
                return{ x, y };
            }
        }
    }
    assert(false);  //no empty board field avaiable
    return{ 42, 42 };
}