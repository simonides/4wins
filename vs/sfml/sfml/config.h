#pragma once
#pragma warning( disable : 4005 )

#define WORKING_DIR "Resources/"

#define WINDOW_TITLE "4Wins by Jakob M., Sebastian S. and Simon D.   @"

extern const unsigned int WINDOW_HEIGHT_TO_CALCULATE; // to this all pixel values are calculated so better don't alter it
extern const unsigned int WINDOW_WIDTH_TO_CALCULATE;

extern const unsigned int WINDOW_HEIGHT; // standard display res setting
extern const unsigned int WINDOW_WIDTH;

//#define PRINT_ALL

extern const unsigned int MAX_PARTICLE_COUNT;   //Max. number of particles per particleSystem


#define THREAD_DEBUGGING 0                  //prints messages regarding multithreading within the main thread
#define THREAD_DEBUG_MESSAGES 0             //prints messages within other threads; might destroy the output due to concurrency (no flush is used)


#ifdef PRINT_ALL
    #define PRINT_BOARD_TO_CONSOLE 1        
    #define STEP_BY_STEP 1
    #define PRINT_WINNER_PER_ROUND 1

    #define PRINT_THINK_MAP 1
#else
    #define PRINT_BOARD_TO_CONSOLE 0        //prints the board after each step to the console (twice a round)
    #define STEP_BY_STEP 0                  //Waits for user input after each step (twice a round)
    #define PRINT_WINNER_PER_ROUND 0        //prints the winner of each round to the console

    #define PRINT_THINK_MAP 0               //prints the throughts of the AI's to the console
#endif
