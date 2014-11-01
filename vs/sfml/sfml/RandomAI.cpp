#include "RandomAI.h"

#include "BoardState.h"
#include "MeepleBag.h"
#include "Meeple.h"

RandomAI::RandomAI() : I_AI(){
}


RandomAI::~RandomAI(){
}




const Meeple& RandomAI::selectOpponentsMeeple(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board){
    return **(opponentBag.getMeeples());
}

BoardPos RandomAI::selectMeeplePosition(const MeepleBag& ownBag, const MeepleBag& opponentBag, const BoardState& board, const Meeple& meepleToSet){
    for (unsigned int y = 0; y < 4; y++){
        for (unsigned int x = 0; x < 4; x++){
            if (board.isFieldEmpty({ x, y })){
                return{ x, y };
            }
        }
    }
}