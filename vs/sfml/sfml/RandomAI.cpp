#include "RandomAI.h"

#include <assert.h>
#include <algorithm>
#include "BoardState.h"
#include "MeepleBag.h"
//#include "Meeple.h"



const Meeple& RandomAI::selectOpponentsMeeple(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board){
    std::set<Meeple*>::const_iterator it = opponentBag.getMeeples();
    advance(it, rand() % opponentBag.getMeepleCount());
    return **it;
}

BoardPos RandomAI::selectMeeplePosition(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board, const Meeple& meepleToSet){
    BoardPos pos;
    do{
        pos.x = rand() % 4;
        pos.y = rand() % 4;
    } while (!board.isFieldEmpty(pos));    
    return pos;
}