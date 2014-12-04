//This file is not perfectly well written, but it's good enough for this game.

#include "getopt.h"

#include <iostream>
#include <string>
#include <assert.h>
#include <stdint.h>

#include "GameSettings.h"



bool strcmpci(const char* string, const char* compare, size_t count = SIZE_MAX){      //case insensitive string comparison
    size_t compLen = strlen(compare);
    if (count == SIZE_MAX){
        count = compLen + 1;     //also check for string termination
    }
    assert(count <= compLen + 1);
    if (strlen(string) < count - 1){
        return false;
    }

    for (size_t i = 0; i < count; ++i){
        if (string[i] != compare[i]){
            if ((string[i] | 0x20) >= 'a' && (string[i] | 0x20) <= 'z' && (string[i] | 0x20) == (compare[i] | 0x20)){    //compare lower case with upper case
                continue;
            }
            return false;
        }
    }
    return true;
}


void print_usage(char *programName){            //Prints Information on how to use the program
    std::cout << "Usage: " << programName << "<options>" << std::endl;
    std::cout << "      [-sim=number]      Starts the game simulator without graphical output. Also defines the number of games that should be simulated." << std::endl;
    std::cout << "      [-t]               The simulator uses a thread for each AI." << std::endl;
    std::cout << "      [-f]               Fast. The AI doesn't perform a sleep before it's tasks." << std::endl;
    std::cout << "      [-i]               Immediate. The AI's meeples are not slowly moved to the board. They will be positioned immediately." << std::endl;
    std::cout << "      [-m]               Muted. The game will run silent and will not produce any sound." << std::endl;
    std::cout << "      -p1=palyerName" << std::endl;
    std::cout << "      -p2=playerName     Defines the players which are playing against each other." << std::endl;
    std::cout << "                         Possible players:    stupid" << std::endl;
    std::cout << "                                              random" << std::endl;
    std::cout << "                                              thinking" << std::endl;
    std::cout << "                                              smart" << std::endl;
}


GameSettings* parseConsoleParameters(int argc, char *argv[]){   //creates a settings-object according to the program parameters. returns nullptr if it fails. settings need to be deleted()
    GameSettings* settings = new GameSettings;

    // -- helper vars --
    bool interpreted;
    char optStr[5] = { "-p?=" };
    // -- -- -- -- -- --

    for (int i = 1; i < argc; ++i){
        if (strcmpci(argv[i], "-t")){       //simulator uses thready
            settings->threadedSimulator = true;
            continue;
        }
        if (strcmpci(argv[i], "-f")){       //fast
            settings->fast = true;
            continue;
        }
        if (strcmpci(argv[i], "-i")){       //no AI movement interpolation
            settings->noAIsim = true;
            continue;
        }
        if (strcmpci(argv[i], "-m")){       //no sound
            settings->musicMuted = true;
			settings->effectsMuted = true;
            continue;
        }
        if (strcmpci(argv[i], "-sim=", 5)){
            char* simCount = argv[i] + 5;

            settings->simulator = strtol(simCount, nullptr, 10);      //I ignore the fact that letters after the number are treated as valid
            if (settings->simulator == LONG_MAX || settings->simulator == LONG_MIN || settings->simulator<1 || settings->simulator > 1000000){
                std::cout << "Option \"-sim=\" has an invalid value. The content needs to be an integer between 1 and 1,000,000." << std::endl;
                delete settings;
                return nullptr;
            }
            continue;
        }

        interpreted = false;
        for (uint8_t pNr = 0; pNr < 2; ++pNr){        //Check if the AI is given
            optStr[2] = pNr + '1';

            if (strcmpci(argv[i], optStr, strlen(optStr))){
                char* player_cstr = argv[i] + strlen(optStr);

                if (strcmpci(player_cstr, "stupid")){
                    settings->playerType[pNr] = GameSettings::STUPID_AI;
                }
                else if (strcmpci(player_cstr, "random")){
                    settings->playerType[pNr] = GameSettings::RANDOM_AI;
                }
                else if (strcmpci(player_cstr, "thinking")){
                    settings->playerType[pNr] = GameSettings::THINKING_AI;
                }
                else if (strcmpci(player_cstr, "smart")){
                    settings->playerType[pNr] = GameSettings::SMART_AI;
                }else{
                    std::cout << "Option " << optStr << " has an invalid value: unknown AI \"" << player_cstr << "\"" << std::endl;
                    delete settings; 
                    return nullptr;
                }
                
                interpreted = true;
                break;
            }
        }
        if (interpreted){ continue; }

        std::cout << "Unknown option " << argv[i] << std::endl;
        delete settings;
        return nullptr;
    }

    if (settings->simulator > 0 && (settings->playerType[0] == GameSettings::HUMAN || settings->playerType[1] == GameSettings::HUMAN)){
        std::cout << "Incompatible settings. Set player 1 and 2 to something different than a Human. Humans can't be simulated." << std::endl;
        delete settings;
        return nullptr;
    }

    return settings;
}
