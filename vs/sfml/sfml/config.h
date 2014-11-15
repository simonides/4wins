#pragma once
#pragma warning( disable : 4005 )

#define WORKING_DIR std::string("Resources/")

extern const unsigned int WINDOW_HEIGHT;
extern const unsigned int WINDOW_WIDTH;

//#define PRINT_ALL

       


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


//Weitere improvements:
//
//  - gegnerische felder blockieren, wenn der Gegner mit dem nächsten zug gewinnen könnte!
//
