#include "GameSettings.h"

#include <assert.h>

#include "ThreadController.h"


#include "StupidAI.h"
#include "RandomAI.h"
#include "ThinkingAI.h"
#include "SmartAI.h"


GameSettings::GameSettings() : fast(false), noAIsim(false), muted(false), option1(true), option2(true){
}

void GameSettings::setPlayerType(uint8_t playerNum, PlayerType player){
    assert(playerNum < 2);
    playerType[playerNum] = player;
}

GameSettings::PlayerType GameSettings::getPlayerType(uint8_t playerNum) const{
    assert(playerNum < 2);
    return playerType[playerNum];
}

void GameSettings::setPlayerOptions(bool option1, bool option2){
    this->option1 = option1;
    this->option2 = option2;
}

void GameSettings::setAvatar(uint8_t playerNum, ResourceManager::ResourceRect avatar){
    this->avatar[playerNum] = avatar;
}

void GameSettings::setOptions(bool fast, bool noAIsim, bool muted){
    this->fast = fast;
    this->noAIsim = noAIsim;
    this->muted = muted;
}

Player* GameSettings::createPlayer(uint8_t playerNum) const{
    Player* p = new Player();

    p->playerAvatar = avatar[playerNum];

    if (playerType[playerNum] == HUMAN){
        p->type = Player::HUMAN;
    }else{
        p->type = Player::TC;
        I_Player* i_player;
        switch (playerType[playerNum]){
            case STUPID_AI:     i_player = new StupidAI();  break;
            case RANDOM_AI:     i_player = new RandomAI();  break;
            case THINKING_AI:   i_player = new ThinkingAI(option1, option2);    break;
            case SMART_AI:      i_player = new SmartAI(option1, option2);       break;
            default: assert(false); i_player = new StupidAI();  break;
        }
        p->controller = new ThreadController(*i_player);
    }
    return p;
}