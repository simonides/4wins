#include "GameSettings.h"

#include <assert.h>

#include "ThreadController.h"


#include "StupidAI.h"
#include "RandomAI.h"
#include "ThinkingAI.h"
#include "SmartAI.h"


GameSettings::GameSettings() : simulator(0), threadedSimulator(false), fast(false), noAIsim(false), muted(false){
    playerType[0] = HUMAN;
	playerType[1] = SMART_AI;
    avatar[0] = ResourceManager::PROFESSOR_JENKINS;
    avatar[1] = ResourceManager::SMOOTH_STEVE;
    aiOptions[0] = { true, true };
    aiOptions[1] = { true, true };
}


I_Player* createI_Player(const GameSettings& settings, uint8_t playerNum){
    assert(settings.playerType[playerNum] != GameSettings::HUMAN);      //There is no I_Player for a human

    switch (settings.playerType[playerNum]){
    case GameSettings::STUPID_AI:     return new StupidAI();
    case GameSettings::RANDOM_AI:     return new RandomAI();
    case GameSettings::THINKING_AI:   return new ThinkingAI(settings.aiOptions[playerNum].useIntelligentMeepleChoosing, settings.aiOptions[playerNum].useIntelligentMeeplePositioning);
    case GameSettings::SMART_AI:      return new SmartAI(settings.aiOptions[playerNum].useIntelligentMeepleChoosing, settings.aiOptions[playerNum].useIntelligentMeeplePositioning);
    default: assert(false);           return new StupidAI();
    }
}


Player* createPlayer(const GameSettings& settings, uint8_t playerNum){
    Player* p = new Player();

    p->playerAvatar = settings.avatar[playerNum];

    if (settings.playerType[playerNum] == GameSettings::HUMAN){
        p->type = Player::HUMAN;
    }else{
        p->type = Player::TC;
        I_Player* i_player = createI_Player(settings, playerNum);
        p->controller = new ThreadController(*i_player);

        if (settings.fast){
            p->meeplePositionThinkTime = { 0, 0 }; p->meepleChoosingThinkTime = { 0, 0 };
        }else{
            switch (settings.playerType[playerNum]){
                case GameSettings::STUPID_AI:     p->meeplePositionThinkTime = { 0, 0.4 };      p->meepleChoosingThinkTime = { 0, 0.4 };    break;
                case GameSettings::RANDOM_AI:     p->meeplePositionThinkTime = { 0.5, 1.5 };    p->meepleChoosingThinkTime = { 0, 1 };      break;
                case GameSettings::THINKING_AI:   p->meeplePositionThinkTime = { 0.8, 2.2 };    p->meepleChoosingThinkTime = { 0.5, 1.8 };  break;
                case GameSettings::SMART_AI:      p->meeplePositionThinkTime = { 1, 3 };        p->meepleChoosingThinkTime = { 1, 2 };      break;
                default: assert(false);           p->meeplePositionThinkTime = { 0, 0 };        p->meepleChoosingThinkTime = { 0, 0 };      break;
            }
        }
    }
    return p;
}
