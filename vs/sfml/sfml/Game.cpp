#include "Game.h"

#include <vector>
#include <assert.h>
#include <string>
//#include <iostream>
#include <math.h>

#include <SFML/System.hpp>
#include <SFML/System/Clock.hpp>

#include "SoundManager.h"

#include "Meeple.h"
#include "MeepleBag.h"
#include "Board.h"

#include "RBoard.h"
#include "RBag.h"
#include "RMeeple.h"

#include "I_Player.h"
#include "ThreadController.h"

#include "config.h"
#include "helper.h"

#include "ParticleSystem.h"
#include "ResourceManager.h"
#include "RTextManager.h"

#include "RBackground.h"
#include "RGameMenu.h"


#include "Player.h"

#define M_PI       3.14159265358979323846f


#define DUST_X_OFFSET -10     //X-Offset for producing dust when placing a meeple
#define OH_YEAH_PROPABILITY 25
#include <iostream>


Game::Game(sf::RenderWindow& window, Player* _players[2], ResourceManager& resourceManager, SoundManager& soundManager) 
	: window(&window)
	, resourceManager(&resourceManager)
	, soundManager(&soundManager)
	, textManager(new RTextManager(resourceManager))
	, gameMenu(new RGameMenu(resourceManager,soundManager,*textManager))
		
	, background(new RBackground(resourceManager, *_players[0], *_players[1]))
	, backgroundMusic(nullptr)

	, activePlayerIndex(1)
	, logicalBoard(new Board())
	, board(new RBoard(*resourceManager.getTexture(ResourceManager::BOARD_TEX),
                       *resourceManager.getTexture(ResourceManager::FIELD_TEX),
                       *resourceManager.getTexture(ResourceManager::FIELD_TEX)))
	, meeplesToDrawAndSort()
	, HOVERED_MEEPLE_GLOW_COLOR(sf::Color::Yellow)
	, SELECTED_MEEPLE_GLOW_COLOR(sf::Color::Red)
	, particleSystem( new ParticleSystem(*resourceManager.getTexture(ResourceManager::PARTICLE_SPRITE), sf::Vector2u(4, 2)) )
	, dustBuilder( new ParticleBuilder( { 300, 300 }, { 5, 30 }) )
	, mouseCursorParticleBuilder( new ParticleBuilder({ 300, 300 }, { 5, 30 }) )
	, endScreenParticleBuilder( new ParticleBuilder({ 0, static_cast<float>(WINDOW_HEIGHT_TO_CALCULATE) }, { 10, 30 }) )
	
	, selectedMeeple(nullptr)
	, selectedBoardPos()
	, firstFrameOfState(false)
	, hoveredMeeple(nullptr)
	, draggingMeeple(false)
	, draggedMeepleMouseOffset()
	, originalMeeplePosition()

	, MOVE_MEEPLE_ANIMATION_SPEED(800.f)
	, MOVE_MEEPLE_ANIMATION_MAX_LIFT_DISTANCE({ 5.f, 12.f })
	, initialPosition()
	, targetPosition()
	, moveMeepleAnimationDistance(0)
	, moveMeepleAnimationProgress(0)
	, moveMeepleAnimationMaxLiftDistance(0)
	
	, remainingThinkTime(0)
{    
    assert(_players[0] != _players[1]);   //hehehe, that won't crash this game

    players[0] = _players[0];
	players[0]->logicalMeepleBag = new MeepleBag(MeepleColor::WHITE);
	players[0]->rbag = new RBag();

    players[1] = _players[1];
	players[1]->logicalMeepleBag = new MeepleBag(MeepleColor::BLACK);
	players[1]->rbag = new RBag();
    	
    gameStates[0] = new GameState(players[0]->logicalMeepleBag, players[1]->logicalMeepleBag, logicalBoard);
    gameStates[1] = new GameState(players[1]->logicalMeepleBag, players[0]->logicalMeepleBag, logicalBoard);
    	
    initMeeples();
    	    
    //Init Particle systems:   
        dustBuilder->setSprites({ 2, 3 }, { 0, 1 })
            ->setPath({ 30, 110 }, { 270, 340 })
            ->setGravity(4.f, 90)
            ->setRotation()
            ->setFadeoutSpeed({ 300, 500 });   
        mouseCursorParticleBuilder->setSprites({ 0, 3 }, { 0, 1 })
            ->setPath({ 50, 150 }, { 230, 380 })
            ->setGravity(5, 90)
            ->setRotation({ 0, 600 }, { 0.1, 3.5 });   
        endScreenParticleBuilder->setSprites({ 0, 2 }, { 0, 2 })
            ->setDynamicColor()
            ->setPath({ 10, 200 }, { 275, 350 })
            ->setGravity(0)
            ->setRotation({ 100, 600 }, { -1, 3 })
            ->setFadeoutSpeed({ 35, 55 });
}


Game::~Game(){
    delete gameStates[1];
    delete gameStates[0];
	delete players[1]->rbag;	//	deletes all rendermeeples for p2
    delete players[1]->logicalMeepleBag;
	delete players[0]->rbag;	//	deletes all rendermeeples for p1
    delete players[0]->logicalMeepleBag;
    delete endScreenParticleBuilder;
    delete mouseCursorParticleBuilder;
    delete dustBuilder;
    delete particleSystem;
    delete board;				//	deletes all fields
	delete logicalBoard;
	delete background;
	delete textManager;
}


void Game::initMeeples(){
    for (uint8_t bag = 0; bag < 2; ++bag){
        for (uint8_t m = 0; m < 8; ++m){    
            RMeeple* rmeeple = new RMeeple(*players[bag]->logicalMeepleBag->getMeeple(m),
                                           *resourceManager->getTexture(ResourceManager::MEEPLE_SPRITE),
                                           *resourceManager->getTexture(ResourceManager::MEEPLE_GLOW_SPRITE));
            meeplesToDrawAndSort.push_back(rmeeple);
            players[bag]->rbag->addRMeeple(rmeeple);
        }
    }
}


void Game::reset(){
    switchActivePlayer(); // TODO check this.. 

	logicalBoard->reset();
	players[0]->logicalMeepleBag->reset();
	players[0]->rbag->reset();
	players[1]->logicalMeepleBag->reset();
	players[1]->rbag->reset();

	gameMenu->reset();

    selectedMeeple = nullptr;
    firstFrameOfState = true;

    hoveredMeeple = nullptr;
    draggingMeeple = false;    
    
    for (uint8_t i = 0; i < 4; ++i){
        if (winningCombiRMeeples[i] == nullptr){
            break;
        }
        winningCombiRMeeples[i]->setGlow(nullptr);
        winningCombiRMeeples[i] = nullptr;
    }
}

//Game Loop for one game, until there is a winner or the board is full
GameMenuDecision::Enum Game::runGame(){
	sf::Clock clock;
    float elapsedTime;
	
    reset();

    LoopState lastLoopState = DISPLAY_END_SCREEN;   
	LoopState loopState = INIT_STATE;
	LoopState oldLoopState = INIT_STATE; // for pause menu
    GameMenuDecision::Enum gameMenuDecision = GameMenuDecision::KEEP_PLAYING;
    InputEvents inputEvents;
	//bool gameMenuActive = false; // used to show / hide the game menu

    soundManager->getMusic(SoundManager::GAME_START)->play();

    if (backgroundMusic != nullptr){
        backgroundMusic->stop();
    }
    backgroundMusic = soundManager->getMusic(SoundManager::BACKGROUND);
    backgroundMusic->play();
	
	
    while (window->isOpen() && gameMenuDecision == GameMenuDecision::KEEP_PLAYING){
        elapsedTime = clock.getElapsedTime().asSeconds();
	    float fps = 1.f / elapsedTime;
		clock.restart();
        inputEvents = pollEvents();

        //Snow-Animation
            if (inputEvents.rightMouseButtonPressed){            //It's ok that the number of particles might decrease at lower frame rates
                mouseCursorParticleBuilder->setPosition(inputEvents.mousePosition);
                particleSystem->newParticleCloud(1, *mouseCursorParticleBuilder);
            }
			// TODO maybe change to whitelist!
			if (inputEvents.releasedEscape == true && loopState != DISPLAY_PAUSE_MENU && loopState != DISPLAY_END_SCREEN)
			{
				oldLoopState = loopState;
				loopState = DISPLAY_PAUSE_MENU;
				gameMenu->setMenuState(GameWinner::PAUSE);
				inputEvents.releasedEscape = false;
			}
			if (inputEvents.windowHasFocus){
				switch (loopState)
				{
				case INIT_STATE:                                //todo das stimmt no ned ganz human iplayer und tc !!!!
					loopState = players[activePlayerIndex]->type == Player::HUMAN ? HUMAN_SELECT_MEEPLE : TC_START_SELECT_MEEPLE;
					break;
				case I_PLAYER_SELECT_MEEPLE:					loopState = i_playerSelectMeeple();								break;
				case HUMAN_SELECT_MEEPLE:						loopState = humanSelectMeeple(inputEvents);					break;
				case TC_START_SELECT_MEEPLE:					loopState = tcStartSelectMeeple();								break;
				case TC_WAIT_FOR_SELECTED_MEEPLE:				loopState = tcWaitForSelectedMeeple();							break;
				case HIGHLIGHT_SELECTED_MEEPLE:					loopState = highlightSelectedMeeple(elapsedTime);				break;
				case I_PLAYER_SELECT_MEEPLE_POSITION:			loopState = i_playerSelectMeeplePosition();						break;
				case HUMAN_SELECT_MEEPLE_POSITION:				loopState = humanSelectMeeplePosition(inputEvents);				break;
				case TC_START_SELECT_MEEPLE_POSITION:			loopState = tcStartSelectMeeplePosition();						break;
				case TC_WAIT_FOR_SELECTED_MEEPLE_POSITION:		loopState = tcWaitForSelectedMeeplePosition(); 					break;
				case MOVE_MEEPLE_TO_SELECTED_POSITION:			loopState = moveMeepleToSelectedPosition(elapsedTime);			break;
				case CHECK_END_CONDITION:
					loopState = checkEndCondition();
					if (loopState == DISPLAY_END_SCREEN)
					{
						gameMenuDecision = displayEndscreen(inputEvents, elapsedTime);
					}
					break;
				case DISPLAY_PAUSE_MENU:
					if (inputEvents.releasedEscape == true)
					{
						loopState = oldLoopState;
						inputEvents.releasedEscape = false;
					}
					gameMenu->resetHover();
					gameMenuDecision =  gameMenu->handleClickAndHover(&inputEvents);
					break;

				case DISPLAY_END_SCREEN:			            gameMenuDecision = displayEndscreen(inputEvents, elapsedTime);	break;
				}
			}
		std::string title("4Wins by Jakob M., Sebastian S. and Simon D.   @");
		title.append(std::to_string(fps));
		title.append(" fps");
		window->setTitle(title);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//draw
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		window->clear(sf::Color::White);

		background->update(elapsedTime);
		background->draw(*window);

		board->draw(*window);

		sort(meeplesToDrawAndSort.begin(), meeplesToDrawAndSort.end(), [](RMeeple* a, RMeeple* b){return a->getYPos() < b->getYPos(); });
		for (std::vector<RMeeple*>::iterator it = meeplesToDrawAndSort.begin(); it != meeplesToDrawAndSort.end(); ++it){
			(*it)->draw(*window);
		}

		particleSystem->update(elapsedTime);
		particleSystem->draw(*window);

		textManager->update(elapsedTime);
        if (loopState == DISPLAY_END_SCREEN){
			gameMenu->draw(*window);
		} else
		{
			textManager->drawTodo(*window, todoText, activePlayerIndex);
		}

		if (loopState == DISPLAY_PAUSE_MENU){
			gameMenu->draw(*window);
		}
		window->display();


        if (lastLoopState != loopState){
            firstFrameOfState = true;
            lastLoopState = loopState;
        }else{
            firstFrameOfState = false;
        }
	}
    backgroundMusic->stop();
    return gameMenuDecision;
}

void Game::createMeepleDust(sf::FloatRect fieldBounds){
    dustBuilder->setPosition({ fieldBounds.left - DUST_X_OFFSET, fieldBounds.top + fieldBounds.height / 2 })
               ->setPath({ 50, 150 }, { 220, 250 });  //left dust
    particleSystem->newParticleCloud(20, *dustBuilder);    
    dustBuilder->setPosition({ fieldBounds.left + fieldBounds.width + DUST_X_OFFSET, fieldBounds.top + fieldBounds.height / 2 })
               ->setPath({ 50, 150 }, { 290, 320 }); //right dust
    particleSystem->newParticleCloud(20, *dustBuilder);
}

InputEvents Game::pollEvents(){
    static InputEvents events = { false, false, false, true, false, { 0, 0 } };
	events.releasedEscape = false;

    events.pressedLeftMouse = false;
    events.releasedLeftMouse = false;
    events.mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	
	sf::Event event;
    while (window->pollEvent(event)){
		switch (event.type){
			case sf::Event::MouseButtonPressed:
                switch (event.mouseButton.button){
                    case sf::Mouse::Left:
                        events.pressedLeftMouse = true;
                        break;
                    case sf::Mouse::Right:
                        events.rightMouseButtonPressed = true;
                        break;
                    default: break;
                }
                break;
			case sf::Event::MouseButtonReleased:
				switch (event.mouseButton.button){
				    case sf::Mouse::Left:
                        events.releasedLeftMouse = true;
					    break;
                    case sf::Mouse::Right:
                        events.rightMouseButtonPressed = false;
                        break;
				    default: break;
				}
				break;
			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::Escape )
				{
					events.releasedEscape = true;
				}
				break;
			case sf::Event::Resized:
				handleResizeWindowEvent(window);
				break;

			case sf::Event::Closed:
				window->close();
				break;
            case sf::Event::LostFocus:
                events.windowHasFocus = false;
                backgroundMusic->pause();
                soundManager->getMusic(SoundManager::SHEEP)->play();
                break;
            case sf::Event::GainedFocus:
                events.windowHasFocus = true;
                backgroundMusic->play();
                break;
		}
	}
    return events;
}

void Game::switchActivePlayer(){
	++activePlayerIndex;
	activePlayerIndex %= 2;

	if (activePlayerIndex == 0)
	{
		background->setLeftWindow(false);
		background->setRightWindow(true);
	}
	else
	{
		background->setLeftWindow(true);
		background->setRightWindow(false);
	}
}





Game::LoopState Game::i_playerSelectMeeple(){
	assert(players[activePlayerIndex]->type == Player::I_PLAYER);
	todoText = RTextManager::GameAction::CHOOSE_A_MEEPLE;
    const Meeple* meeple = &players[activePlayerIndex]->player->selectOpponentsMeeple(*gameStates[activePlayerIndex]);

    selectedMeeple = players[(activePlayerIndex+1)%2]->rbag->getRmeepleFromUnused(meeple);
    assert(selectedMeeple != nullptr);
	return HIGHLIGHT_SELECTED_MEEPLE;
}

Game::LoopState Game::humanSelectMeeple(InputEvents inputEvents){
    assert(players[activePlayerIndex]->type == Player::HUMAN);
	todoText = RTextManager::GameAction::CHOOSE_A_MEEPLE;

    if (inputEvents.releasedLeftMouse){
        selectedMeeple = players[(activePlayerIndex + 1) % 2]->rbag->getRMeepleAtPosition(inputEvents.mousePosition);
        if (selectedMeeple != nullptr){
            selectedMeeple->setGlow(&SELECTED_MEEPLE_GLOW_COLOR);

            switchActivePlayer();
            hoveredMeeple = nullptr;
            soundManager->getMusic(SoundManager::SELECT)->play();
            if (players[activePlayerIndex]->type == Player::TC){
                return TC_START_SELECT_MEEPLE_POSITION;
            }
            if (players[activePlayerIndex]->type == Player::HUMAN) {
                return HUMAN_SELECT_MEEPLE_POSITION;
            }
            return I_PLAYER_SELECT_MEEPLE_POSITION;
        }
    }
    if (hoveredMeeple != nullptr){
        hoveredMeeple->setGlow(nullptr);
    }
    hoveredMeeple = players[(activePlayerIndex + 1) % 2]->rbag->getRMeepleAtPosition(inputEvents.mousePosition);
    if (hoveredMeeple != nullptr){
        hoveredMeeple->setGlow(&HOVERED_MEEPLE_GLOW_COLOR);
    }
    return HUMAN_SELECT_MEEPLE;

}

Game::LoopState Game::tcStartSelectMeeple(){
	todoText = RTextManager::GameAction::CHOOSE_A_MEEPLE;
	assert(players[activePlayerIndex]->type == Player::TC);
	players[activePlayerIndex]->controller->run_selectOpponentsMeeple(*gameStates[activePlayerIndex]);
	return TC_WAIT_FOR_SELECTED_MEEPLE;
}

Game::LoopState Game::tcWaitForSelectedMeeple(){
	assert(players[activePlayerIndex]->type == Player::TC);
	if (!players[activePlayerIndex]->controller->isResultAvailable()){
		return TC_WAIT_FOR_SELECTED_MEEPLE;
	}
    const Meeple* meeple = &players[activePlayerIndex]->controller->getOpponentsMeeple();
    selectedMeeple = players[(activePlayerIndex + 1) % 2]->rbag->getRmeepleFromUnused(meeple);
    assert(selectedMeeple != nullptr);
	return HIGHLIGHT_SELECTED_MEEPLE;
}

Game::LoopState Game::highlightSelectedMeeple(float elapsedTime){
    assert(players[activePlayerIndex]->type == Player::TC || players[activePlayerIndex]->type == Player::I_PLAYER);
    assert(selectedMeeple != nullptr);

    if (firstFrameOfState){        
        remainingThinkTime = players[activePlayerIndex]->meepleChoosingThinkTime.get();
    }
    if (remainingThinkTime > 0){                //Time, before the AI starts moving the meeple
        remainingThinkTime -= elapsedTime;
        return HIGHLIGHT_SELECTED_MEEPLE;
    }   
	 	
	selectedMeeple->setGlow(&SELECTED_MEEPLE_GLOW_COLOR);
    soundManager->getMusic(SoundManager::SELECT)->play();
	switchActivePlayer();
    return players[activePlayerIndex]->type == Player::HUMAN ? HUMAN_SELECT_MEEPLE_POSITION : TC_START_SELECT_MEEPLE_POSITION;
}

Game::LoopState Game::i_playerSelectMeeplePosition(){
	assert(players[activePlayerIndex]->type == Player::I_PLAYER);
	todoText = RTextManager::GameAction::SELECT_MEEPLE_POS;
    selectedBoardPos = players[activePlayerIndex]->player->selectMeeplePosition(*gameStates[activePlayerIndex], *(selectedMeeple->getLogicalMeeple()));
	return MOVE_MEEPLE_TO_SELECTED_POSITION;
}

Game::LoopState Game::humanSelectMeeplePosition(InputEvents inputEvents){
	assert(players[activePlayerIndex]->type == Player::HUMAN);
	assert(selectedMeeple != nullptr);
	todoText = RTextManager::GameAction::SELECT_MEEPLE_POS;
	//clicked meeple -> start to drag
    if (inputEvents.pressedLeftMouse && selectedMeeple->containsPosition(inputEvents.mousePosition)){
		originalMeeplePosition = selectedMeeple->getPosition();
        draggedMeepleMouseOffset = selectedMeeple->getMousePosRelativeToMeepleBoundary(inputEvents.mousePosition);
		draggingMeeple = true;
	}

	if (draggingMeeple){ // todo checken ob !releaseleftmous braucht
        sf::Vector2f test(inputEvents.mousePosition.x - draggedMeepleMouseOffset.x, inputEvents.mousePosition.y - draggedMeepleMouseOffset.y);
		selectedMeeple->setPosition(test);
		sf::Vector2f lookupPos = selectedMeeple->getCoords();
		board->setHoveredField(board->getBoardPosForPosititon(lookupPos));
	}

    if (inputEvents.releasedLeftMouse && draggingMeeple){
		draggingMeeple = false;
		sf::Vector2f lookupPos = selectedMeeple->getCoords();
		BoardPos pos = board->getBoardPosForPosititon(lookupPos);
		if ((pos.x < 4 && pos.y < 4) && logicalBoard->isFieldEmpty(pos)){
            /*The following code can be used to change the sound volume + dust number relative to the distance of the meeple <-> field...
            sf::Vector2f meepleDropOffCoords = selectedMeeple->getPosition();
            sf::Vector2f fieldCoords = board->getFieldCoords(pos);
            sf::Vector2f diff(fieldCoords.x - meepleDropOffCoords.x, fieldCoords.y - meepleDropOffCoords.y);
            float distanceToField = sqrt(diff.x * diff.x + diff.y * diff.y);*/
            selectedMeeple->setPosition(board->getFieldCoords(pos));
			players[activePlayerIndex]->rbag->changeRMeepleToUsed(*selectedMeeple);

            Meeple* placeMe = players[activePlayerIndex]->logicalMeepleBag->removeMeeple(*(selectedMeeple->getLogicalMeeple()));
			logicalBoard->setMeeple(pos, *placeMe);
			selectedMeeple->setGlow(nullptr);
			selectedMeeple = nullptr;
			board->setHoveredField({ 42, 42 });
            soundManager->getMusic(SoundManager::MEEPLE_DROP)->play();
            if (rand()%100 <= OH_YEAH_PROPABILITY){
                soundManager->getMusic(SoundManager::OH_YEAH)->play();
            }
            createMeepleDust(board->getFieldGlobalBounds(pos));
			return CHECK_END_CONDITION;
		}
		selectedMeeple->setPosition(originalMeeplePosition);
	}
	return HUMAN_SELECT_MEEPLE_POSITION;
}

Game::LoopState Game::tcStartSelectMeeplePosition(){
	assert(players[activePlayerIndex]->type == Player::TC);
	todoText = RTextManager::GameAction::SELECT_MEEPLE_POS;
    players[activePlayerIndex]->controller->run_selectMeeplePosition(*gameStates[activePlayerIndex], *(selectedMeeple->getLogicalMeeple()));
	return TC_WAIT_FOR_SELECTED_MEEPLE_POSITION;
}

Game::LoopState Game::tcWaitForSelectedMeeplePosition(){
	assert(players[activePlayerIndex]->type == Player::TC);
	if (!players[activePlayerIndex]->controller->isResultAvailable()){
		return TC_WAIT_FOR_SELECTED_MEEPLE_POSITION;
	}
	selectedBoardPos = players[activePlayerIndex]->controller->getMeeplePosition();
	return MOVE_MEEPLE_TO_SELECTED_POSITION;
}

Game::LoopState Game::moveMeepleToSelectedPosition(float elapsedTime){
    assert(players[activePlayerIndex]->type == Player::TC || players[activePlayerIndex]->type == Player::I_PLAYER);
	assert(selectedMeeple != nullptr);
	assert(selectedBoardPos.x < 4 && selectedBoardPos.y < 4);

    if (firstFrameOfState){
        initialPosition = selectedMeeple->getPosition();
        targetPosition = board->getFieldCoords(selectedBoardPos);
        moveMeepleAnimationProgress = 0;

        sf::Vector2f delta = { targetPosition.x - initialPosition.x, targetPosition.y - initialPosition.y };
        moveMeepleAnimationDistance = sqrt(delta.x*delta.x + delta.y * delta.y);

        moveMeepleAnimationMaxLiftDistance = MOVE_MEEPLE_ANIMATION_MAX_LIFT_DISTANCE.get();
        remainingThinkTime = players[activePlayerIndex]->meeplePositionThinkTime.get();
    }
	   
    if (remainingThinkTime > 0){                //Time, before the AI starts moving the meeple
        remainingThinkTime -= elapsedTime;
        return MOVE_MEEPLE_TO_SELECTED_POSITION;
    }

    float speedModulation = sin(0.1f* M_PI + moveMeepleAnimationProgress * M_PI * 0.8f) * 0.6f + 0.2f;
    moveMeepleAnimationProgress += MOVE_MEEPLE_ANIMATION_SPEED * elapsedTime * (1.f/moveMeepleAnimationDistance) * speedModulation * speedModulation;          //speedModulation: decrease the speed at the beginning and ending
    
    sf::Vector2f position;
        position.x = moveMeepleAnimationProgress * targetPosition.x + (1.f - moveMeepleAnimationProgress) * initialPosition.x;
        position.y = moveMeepleAnimationProgress * targetPosition.y + (1.f - moveMeepleAnimationProgress) * initialPosition.y;       
        position.y -= sin(moveMeepleAnimationProgress * M_PI) * moveMeepleAnimationDistance / moveMeepleAnimationMaxLiftDistance;       //Simulate, that the opponent "lifted" the meeple

    selectedMeeple->setPosition(position);
	
    if (moveMeepleAnimationProgress >= 1){        
        selectedMeeple->setGlow(nullptr);

        Meeple* placeMe = players[activePlayerIndex]->logicalMeepleBag->removeMeeple(*(selectedMeeple->getLogicalMeeple())); //Remove the meeple from the bag
        players[activePlayerIndex]->rbag->changeRMeepleToUsed(*selectedMeeple);
        logicalBoard->setMeeple(selectedBoardPos, *placeMe);

        selectedMeeple->setPosition(targetPosition);
        soundManager->getMusic(SoundManager::MEEPLE_DROP)->play();
        if (rand() % 100 <= OH_YEAH_PROPABILITY){
            soundManager->getMusic(SoundManager::OH_YEAH)->play();
        }
        createMeepleDust(board->getFieldGlobalBounds(selectedBoardPos));
        return CHECK_END_CONDITION;
    }

	return MOVE_MEEPLE_TO_SELECTED_POSITION;
}

Game::LoopState Game::checkEndCondition(){
	#if PRINT_BOARD_TO_CONSOLE
		std::cout << "Player " << activePlayerIndex + 1 << " chose meeple \"" << selectedMeeple->toString() << '\"' << std::endl;
		std::cout << "Player " << (activePlayerIndex + 1) % 2 + 1 << " sets meeple to " << selectedMeeple->toString() << std::endl;
		logicalBoard->print(std::cout);
	#endif
	#if PRINT_WINNER_PER_ROUND
		std::cout << "Player " << activePlayerIndex + 1 << " wins!" << std::endl;
	#endif
            
    //The meeple has just been placed. Sound is played soon
	selectedMeeple = nullptr;
    
	const WinCombination* combi = logicalBoard->checkWinSituation();
	
    if (combi != nullptr){
		for (uint8_t i = 0; i < 4; ++i){
			RMeeple* temp = players[activePlayerIndex]->rbag->getRmeepleFromUsed(combi->meeples[i]);
			if (temp == nullptr){
				winningCombiRMeeples[i] = players[(activePlayerIndex + 1) % 2]->rbag->getRmeepleFromUsed(combi->meeples[i]);
			}else{
				winningCombiRMeeples[i] = temp;
			}
			assert(winningCombiRMeeples[i] != nullptr);
		}
        soundManager->getMusic(SoundManager::MEEPLE_WIN_DROP)->play();
		gameMenu->setMenuState((activePlayerIndex == 0) ? GameWinner::PLAYER_1 : GameWinner::PLAYER_2);
		return DISPLAY_END_SCREEN;
	}
    
	if (logicalBoard->isFull()){
	    #if PRINT_WINNER_PER_ROUND
			    std::cout << "Tie! There is no winner." << std::endl;
	    #endif
		gameMenu->setMenuState(GameWinner::TIE);
		return DISPLAY_END_SCREEN;
	}
	return INIT_STATE;
}

GameMenuDecision::Enum Game::displayEndscreen(InputEvents inputEvents, float elapsedTime){

	if (firstFrameOfState || rand() % 100 < 70){
		int particle_count = 1;
		endScreenParticleBuilder->setPosition({ 0, static_cast<float>(WINDOW_HEIGHT_TO_CALCULATE) }, { 5, 30 })
			->setPath({ 10, 200 }, { 275, 350 });
		particleSystem->newParticleCloud(particle_count, *endScreenParticleBuilder);
		endScreenParticleBuilder->setPosition({ static_cast<float>(WINDOW_WIDTH_TO_CALCULATE), static_cast<float>(WINDOW_HEIGHT_TO_CALCULATE) })
			->setPath({ 10, 200 }, { 190, 265 });
		particleSystem->newParticleCloud(particle_count, *endScreenParticleBuilder);
		endScreenParticleBuilder->setPosition({ 0, 0 })
			->setPath({ 10, 200 }, { -15, 89 });
		particleSystem->newParticleCloud(particle_count, *endScreenParticleBuilder);
		endScreenParticleBuilder->setPosition({ static_cast<float>(WINDOW_WIDTH_TO_CALCULATE), 0 })
			->setPath({ 10, 200 }, { 89, 195 });
		particleSystem->newParticleCloud(particle_count, *endScreenParticleBuilder);
	}
	if (firstFrameOfState){
		for (uint8_t i = 0; i < 4; i++){
			colorAnimations[i].init(4, i * 20);
		}
		backgroundMusic->stop();
		backgroundMusic = soundManager->getMusic(SoundManager::WIN_MUSIC);
		backgroundMusic->play();
		//backgroundMusic->setPlayingOffset(sf::seconds(36.f)); //TODO change file so it fits.. 
	}

	//Animate rainbow-colors on the meeples, which were responsible for the winning-combination:  
	if (logicalBoard->checkWinSituation() != nullptr){ // if this is false it is a tie!
		sf::Color color;
		for (uint8_t i = 0; i < 4; ++i){
			colorAnimations[i].animate(elapsedTime);
			color = colorAnimations[i].getColor();
			winningCombiRMeeples[i]->setGlow(&color);
		}
	}
	//Handle GameMenu Buttons
	gameMenu->resetHover();
	return gameMenu->handleClickAndHover(&inputEvents);
}
