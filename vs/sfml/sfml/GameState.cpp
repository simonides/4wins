#include "GameSimulator.h"

#include "MeepleBag.h"



GameState::GameState(const MeepleBag* ownBag, const MeepleBag* opponentBag, const Board* board) :
    ownBag(ownBag), opponentBag(opponentBag), board(board), cloned(false){
}

GameState::GameState(const GameState& base) :
    ownBag(new MeepleBag(*base.ownBag)), opponentBag(new MeepleBag(*base.opponentBag)), board(new Board(*base.board, ownBag, opponentBag)), cloned(true){
}

GameState::~GameState(){
    if (cloned){
        delete ownBag;
        delete opponentBag;
        delete board;
    }
}