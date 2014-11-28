#pragma once


#include <thread>
#include <mutex>
#include <condition_variable>
#include "Board.h"
#include "I_Player.h"

class GameState;
    

struct ThreadCommand{                       //A command that is transmitted from the main thread to the player thread in order to tell him what to do
    enum Type{
        SELECT_OPPONENTS_MEEPLE,
        SELECT_MEEPLE_POSITION,
        RESET_PLAYER,
        TERMINATE
    } type;

    const GameState* gameState;
    const Meeple* meepleToSet;          //Only needed for the type "SELECT_MEEPLE_POSITION"

    std::string toString();
};


//Note: while the thread is performing tasks, the state of the board-object MUST NOT BE MODIFIED
class ThreadController{
private:
    
//Main thread only:
    std::thread* playerThread; 
    bool threadAlive;                       //true, as long as the thread is alive (thread can be started by calling initialiseThread)
    void initialiseThread();                //Starts the thread - lazy call (the thread is generated as soon as the caller requires it)   

//Thread Interface:
    std::mutex mutex;
    std::unique_lock<std::mutex> lock;      //for main thread only 
    std::condition_variable cv;             //Is used similar like a messaging-mechanism
    //send task-to-thread:
        ThreadCommand command;              //Contains the command, which should be performed by the thread
        bool commandAvailable;              //true: there's a new command for the player-thread, which hasn't completed yet (also true while the thread is working on it)
    //get result-from-thread:
        const Meeple* opponentsMeeple;          
        BoardPos meeplePosition;                
        bool resultAvailable;               //true: The thread finished its task
               
//Thread-only:
    static void threadMainRoutine(ThreadController* controller);       //This is the function, which is being executed by the thread
    I_Player* player;                       //The player's functions are called within the thread
    
// ~ ~ ~ ~
    ThreadController(ThreadController&);    //no copy-constructor allowed
public:
    ThreadController(I_Player& player);     //the TC now owns the I_Player and will delete it at the end

    bool isResultAvailable();               //Checks, if the thread finished its task

//Start thread-tasks:
	//ai selects meeple for opponent (player)
    void run_selectOpponentsMeeple(const GameState& gameState);
	//tells where to place the meeple chosen by the opponent
    void run_selectMeeplePosition(const GameState& gameState, const Meeple& meepleToSet);
    void run_resetPlayer();
//Get the results of the thread-tasks:
    const Meeple& getOpponentsMeeple();
    BoardPos getMeeplePosition();
    void confirmTaskCompletion();           //Confirms that the thread completed its task, but ignores the result (a new task can be initiated); Has to be called after the player has been reseted
    
    const I_Player* getAssociatedPlayer();  //Returns the address of the associated player; note: the caller should not access the player while the thread is performing tasks

    ~ThreadController();
};

