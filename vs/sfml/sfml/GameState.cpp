#include "GameSimulator.h"

#include "MeepleBag.h"



GameState::GameState(const MeepleBag* ownBag, const MeepleBag* opponentBag, const Board* board) :
    ownBag(ownBag), opponentBag(opponentBag), board(board), cloned(false){
}

GameState::GameState(const GameState& base) :
    ownBag(new MeepleBag(*ownBag)), opponentBag(new MeepleBag(*opponentBag)), board(new Board(*board, ownBag, opponentBag)), cloned(true){
}

GameState::~GameState(){
    if (cloned){
        delete ownBag;
        delete opponentBag;
        delete board;
    }
}