#include "ThreadController.h"

#include <assert.h>
#include <iostream>

#include "Board.h"
#include "config.h"
#include "Meeple.h"


ThreadController::ThreadController(I_Player& player) : playerThread(nullptr),
                                                       threadAlive(false), 
                                                       lock(mutex, std::defer_lock),        //defer_lock = don't lock the mutex
                                                       command({ ThreadCommand::TERMINATE, nullptr, nullptr }),
                                                       commandAvailable(false),
                                                       opponentsMeeple(nullptr), 
                                                       meeplePosition({ -1, -1 }),
                                                       resultAvailable(false),
                                                       player(&player) {
}

void ThreadController::initialiseThread(){
    if (threadAlive){          //The thread is already running
        return;
    }
    assert(playerThread == nullptr);
    threadAlive = true;
    #if THREAD_DEBUGGING
        std::cout << "starting thread" << std::endl;
    #endif
    playerThread = new std::thread(ThreadController::threadMainRoutine, this);
}

ThreadController::~ThreadController(){
    if (threadAlive){
        lock.lock();       
            #if THREAD_DEBUGGING
                std::cout << "Sending terminate-command to thread" << std::endl;
            #endif           
            resultAvailable = false;
            command = { ThreadCommand::TERMINATE, nullptr, nullptr };
            commandAvailable = true;
        lock.unlock();
        cv.notify_one();

        #if THREAD_DEBUGGING
            std::cout << "Waiting for thread to complete..." << std::endl;
        #endif
        
        playerThread->join();
        #if THREAD_DEBUGGING
            std::cout << "Thread terminated." << std::endl;
        #endif
        delete playerThread;
    }
}

//THIS FUNCTION RUNS IN A SEPERATE THREAD
void ThreadController::threadMainRoutine(ThreadController* controller){
    #if THREAD_DEBUG_MESSAGES
        std::cout << "~~ thread: created ~~ " << std::endl;
    #endif

    //Values to be returned after each task:
        const Meeple* opponentsMeeple = nullptr;
        BoardPos meeplePosition = { 42, 42 };

    std::unique_lock<std::mutex> lock(controller->mutex);       //Acquire a unique lock --> the mutex is now locked

    for (;;){
        #if THREAD_DEBUG_MESSAGES
            std::cout << "~~ thread: waiting for command ~~" << std::endl;
        #endif
        
        //cv.wait requires a unique_lock with a locked mutex.
        //cv then releases the lock, and waits for the condition to become true (wait for the signal)
        //after the condition became true, the mutex is locked again, and the command goes on
        controller->cv.wait(lock, [controller]{return controller->commandAvailable; });     //omg, can't believe that I'm actually using a lambda in c++
            ThreadCommand command = controller->command;
        lock.unlock();      //Unlock - we have everything we need       

        #if THREAD_DEBUG_MESSAGES
            std::cout << "~~ thread: received command " << command.toString() << " ~~" << std::endl;
        #endif
        
        switch (command.type){
            case ThreadCommand::SELECT_OPPONENTS_MEEPLE:    
                opponentsMeeple = &controller->player->selectOpponentsMeeple(*command.gameState);
                meeplePosition = { 42, 42 };
                break;
            case ThreadCommand::SELECT_MEEPLE_POSITION:   
                opponentsMeeple = nullptr;
                meeplePosition = controller->player->selectMeeplePosition(*command.gameState, *command.meepleToSet);
                break;
            case ThreadCommand::RESET_PLAYER:
                opponentsMeeple = nullptr;
                meeplePosition = { 42, 42 };
                controller->player->reset();
                break;
            case ThreadCommand::TERMINATE: 
                return;
        }
                       
        #if THREAD_DEBUG_MESSAGES
            std::cout << "~~ thread: completed the command ~~" << std::endl;
        #endif

        
        lock.lock();        //Lock: we wanna access the shared memory
            assert(!controller->resultAvailable);   //Don't overwrite results before they have been read
            controller->opponentsMeeple = opponentsMeeple;
            controller->meeplePosition = meeplePosition;
            controller->commandAvailable = false;   //command completed
            controller->resultAvailable = true;     //new result avaiable
        //Don't unlock the mutex - we need it for the next cv.wait()                        
        controller->cv.notify_one();
    }
}

bool ThreadController::isResultAvailable(){
    lock.lock();
        bool available = resultAvailable;
		assert(available || commandAvailable);
    lock.unlock();
    return available;
}

void ThreadController::run_selectOpponentsMeeple(const GameState& gameState){
    initialiseThread();                         //Initialise the thread, if it doesn't exist yet  
    lock.lock();    
        assert(!commandAvailable);              //we can't send a new command, if there is still a command in the queue
        assert(!resultAvailable);               //we can't send a new command, if the result from the old one hasn't been read yet.
        
        command = { ThreadCommand::SELECT_OPPONENTS_MEEPLE, &gameState, nullptr };
        
        #if THREAD_DEBUGGING
            std::cout << "Sending new command to thread: " << command.toString() << std::endl;
        #endif
        commandAvailable = true;
    lock.unlock();
    cv.notify_one();                            //notify the thread (the cv.wait() will wake up the thread)
}

void ThreadController::run_selectMeeplePosition(const GameState& gameState, const Meeple& meepleToSet){
    initialiseThread();                         //Initialise the thread, if it doesn't exist yet  
    lock.lock();    
        assert(!commandAvailable);              //we can't send a new command, if there is still a command in the queue
        assert(!resultAvailable);               //we can't send a new command, if the result from the old one hasn't been read yet.
        
        command = { ThreadCommand::SELECT_MEEPLE_POSITION, &gameState, &meepleToSet };
        
        #if THREAD_DEBUGGING
            std::cout << "Sending new command to thread: " << command.toString() << std::endl;
        #endif
        commandAvailable = true;
    lock.unlock();
    cv.notify_one();                            //notify the thread (the cv.wait() will wake up the thread
}   

void ThreadController::run_resetPlayer(){
    initialiseThread();                         //Initialise the thread, if it doesn't exist yet  
    lock.lock();    
        assert(!commandAvailable);              //we can't send a new command, if there is still a command in the queue
        assert(!resultAvailable);               //we can't send a new command, if the result from the old one hasn't been read yet.
        
        command = { ThreadCommand::RESET_PLAYER, nullptr, nullptr };
        
        #if THREAD_DEBUGGING
            std::cout << "Sending new command to thread: " << command.toString() << std::endl;
        #endif
        commandAvailable = true;
    lock.unlock();
    cv.notify_one();  
}

const Meeple& ThreadController::getOpponentsMeeple(){
    lock.lock();
        assert(commandAvailable || resultAvailable);    //calling this function makes no sense if the thread never received a task
    cv.wait(lock, [this]{return resultAvailable; });    //wait, until the results are available
        assert(opponentsMeeple != nullptr);
        const Meeple* copy = opponentsMeeple;
        opponentsMeeple = nullptr;                      //allows the class to check if the user really claims the correct result, and to return wrong values if (s)he isn't
        resultAvailable = false;                        //we read the result
    
        #if THREAD_DEBUGGING
            std::cout << "Received Command Result: Meeple = " << copy->toString() << std::endl;
        #endif
    lock.unlock();
    return *copy;
}

BoardPos ThreadController::getMeeplePosition(){
    BoardPos pos;
    lock.lock();
        assert(commandAvailable || resultAvailable);    //calling this function makes no sense if the thread never received a task
    cv.wait(lock, [this]{return resultAvailable; });    //wait, until the results are available
        assert(meeplePosition.x < 4 && meeplePosition.y < 4);
        pos = meeplePosition;
        meeplePosition = { 42, 42 };                    //allows the class to check if the user really claims the correct result, and to return wrong values if (s)he isn't
        resultAvailable = false;                        //we read the result
    
        #if THREAD_DEBUGGING
            std::cout << "Received Command Result: Position = " << pos.toString() << std::endl;
        #endif
    lock.unlock();
    return pos;
}

void ThreadController::confirmTaskCompletion(){
    lock.lock();
        assert(commandAvailable || resultAvailable);    //calling this function makes no sense if the thread never received a task
    cv.wait(lock, [this]{return resultAvailable; });    //wait, until the results are available
        resultAvailable = false;                        //we read the result
    
        #if THREAD_DEBUGGING
            std::cout << "Received Command Result" << std::endl;
        #endif
    lock.unlock();
}

const I_Player* ThreadController::getAssociatedPlayer(){
    return player;
}
