#include "getopt.h"

#include <iostream>
#include <string>
#include <assert.h>
#include <stdint.h>


//bool strcmpci(std::string string, std::string compare, size_t count = -1){      //case insensitive string comparison
//    size_t compLen = compare.length();
//    if (count < 0){
//        count = compLen;
//    }
//    assert(count <= compLen);
//    if (string.length() < count){
//        return false;
//    }
//    std::transform(string.begin(), string.end(), string.begin(), ::toupper);        //this can be improved - we don't need to modify the whole string
//    std::transform(compare.begin(), compare.end(), compare.begin(), ::toupper);     //this can be improved - we don't need to modify the whole string
//    if (string.length() < count || compare.length() < count){                       //toUpper might change the number of characters (ß --> SS)
//        return false;
//    }
//    for (size_t i = 0; i < count; ++i){
//        if (string[i] != compare[i]){
//            return false;
//        }
//    }
//    return true;
//}




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




void parseConsoleParameters(int argc, char *argv[]){
    std::string ai[2];
    bool fast = false;              //true: the AI's have no sleep-time for thinking anymore, but do their turn asap
    bool noAIsim = false;           //true: the AI's turns are not interpolated anymore (the meeple is placed on the board immediately)
    bool muted = false;             //true: the game will be muted, and no sound will be played
    unsigned int simulator = 0;     //>0: use the simulator instead of the graphical output. Numer = number of games

    // -- helper vars --
    bool interpreted;
    char optStr[5] = { "-p?=" };
    // -- -- -- -- -- --
    for (int i = 1; i < argc; ++i){

        if (strcmpci(argv[i], "-f")){       //fast
            fast = true;
            continue;
        }
        if (strcmpci(argv[i], "-i")){       //no AI movement interpolation
            noAIsim = true;
            continue;
        }
        if (strcmpci(argv[i], "-m")){       //no sound
            muted = true;
            continue;
        }
        if (strcmpci(argv[i], "-sim=", 5)){
            char* simCount = argv[i] + 5;

            simulator = strtol(simCount, nullptr, 10);      //I ignore the fact that letters after the number are treated as valid
            if (simulator == LONG_MAX || simulator == LONG_MIN || simulator<1 || simulator > 1000000){
                std::cout << "Option \"-sim=\" has an invalid value. The content needs to be an integer between 1 and 1,000,000." << std::endl;
                std::cin.ignore();
                exit(1);
            }
            continue;
        }

        interpreted = false;
        for (uint8_t aiNr = 0; aiNr < 2; ++aiNr){        //Check if the AI is given
            optStr[2] = aiNr + '1';

            if (strcmpci(argv[i], optStr, strlen(optStr))){
                char* ai_cstr = argv[i] + strlen(optStr);
                if (ai[aiNr].length() != 0){
                    std::cout << "Option " << optStr << " has been given twice" << std::endl;
                    std::cin.ignore();
                    exit(1);
                }
                ai[aiNr] = ai_cstr;
                interpreted = true;
                break;
            }
        }
        if (interpreted){ continue; }

        std::cout << "Unknown option " << argv[i] << std::endl;
        std::cin.ignore();
        exit(1);
    }

    if (ai[0].length() == 0 || ai[1].length() == 0){
        std::cout << "Missing option \"-ai1=\" and/or \"-ai2=\": unknown players " << std::endl;
        std::cin.ignore();
        exit(1);
    }

    if (simulator != 0 && (fast || noAIsim)){
        std::cout << "The option \"-sim=\" is invalid with the flags -r and -i" << std::endl;
        std::cin.ignore();
        exit(1);
    }

    std::cout << "OPTIONS:" << std::endl;
    std::cout << "   Simulator: " << simulator << std::endl;
    std::cout << "   AI1: " << ai[0] << std::endl;
    std::cout << "   AI2: " << ai[1] << std::endl;
    std::cout << "   muted: " << (muted ? "true" : "false") << std::endl;
    std::cout << "   no AI thinking: " << (fast ? "true" : "false") << std::endl;
    std::cout << "   no AI interpol: " << (noAIsim ? "true" : "false") << std::endl;
    std::cin.ignore();
    exit(0);
}

void print_usage(char *programName){            //Prints Information on how to use the program
    std::cout << "Usage: " << programName << "<options>" << std::endl;
    std::cout << "      [-sim=number]      Starts the game simulator without graphical output. Also defines the number of games that should be simulated." << std::endl;
    std::cout << "      [-f]               Fast. The AI doesn't perform a sleep before it's tasks." << std::endl;
    std::cout << "      [-i]               Immediate. The AI's meeples are not slowly moved to the board. They will be positioned immediately ." << std::endl;
    std::cout << "      [-m]               Muted. The game will run silent and will not produce any sound." << std::endl;
    std::cout << "      -p1=palyerName" << std::endl;
    std::cout << "      -p2=playerName     Defines the players which are playing against each other." << std::endl;
    //TODO: print player names
}
