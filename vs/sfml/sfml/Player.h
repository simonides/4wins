#pragma once
#include "ResourceManager.h"
#include "helper.h"



class I_Player;
class ThreadController;
class MeepleBag;
class RBag;



struct Player{
    enum{
        HUMAN,
        I_PLAYER, //TODO: still not fully supported
        TC
    } type;

    ResourceManager::ResourceRect playerAvatar;

    union{
        ThreadController* controller;
        I_Player* player;
    };
    RBag* rbag;
    MeepleBag* logicalMeepleBag;

    Interval meeplePositionThinkTime;   //For TC/I_Player only: how long the AI thinks about the meeple position
    Interval meepleChoosingThinkTime;   //For TC/I_Player only: how long the AI thinks about the meeple to choose
};
