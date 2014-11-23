#include "Game.h"

#include <vector>
#include <assert.h>
#include <string>
//#include <iostream>
#include <math.h>

#include <SFML/System.hpp>
#include <SFML/System/Clock.hpp>

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
#include "ResourceLoader.h"

#include "RBackground.h"

#define M_PI       3.14159265358979323846f

//
//const sf::Color HOVERED_MEEPLE_GLOW_COLOR = sf::Color::Yellow;
//const sf::Color SELECTED_MEEPLE_GLOW_COLOR = sf::Color::Red;


#define DUST_X_OFFSET -10     //X-Offset for producing dust when placing a meeple

Game::Game(sf::RenderWindow& window, Player* _players[2], ResourceLoader& resourceLoader)
	: window(&window), resourceLoader(&resourceLoader)
	, pressedLeftMouse(false)
	, releasedLeftMouse(false), rMeepleToSet(nullptr)
	, activePlayerIndex(0), HOVERED_MEEPLE_GLOW_COLOR(sf::Color::Yellow)
    , SELECTED_MEEPLE_GLOW_COLOR(sf::Color::Red), firstFrameOfState(false)
    , GAME_MENU_BUTTON_COLOR(sf::Color::White), GAME_MENU_BUTTON_HOVER_COLOR(sf::Color::Magenta)
    , GAME_MENU_BUTTON_SIZE(150.f, 150.f)
	, hoveredButtonPtr(nullptr)
    , dustBuilder(nullptr), endScreenParticleBuilder(nullptr)
    , rightMouseButtonPressed(false)
    , MOVE_MEEPLE_ANIMATION_SPEED(800.f)
    , MOVE_MEEPLE_ANIMATION_MAX_LIFT_DISTANCE({ 5.f, 12.f })
	{
    
    assert(_players[0] != _players[1]);   //hehehe, that won't crash this game

    players[0] = _players[0];
	players[0]->logicalMeepleBag = new MeepleBag(MeepleColor::WHITE);
	players[0]->rbag = new RBag();

    players[1] = _players[1];
	players[1]->logicalMeepleBag = new MeepleBag(MeepleColor::BLACK);
	players[1]->rbag = new RBag();

	logicalBoard = new Board();	

	board = new RBoard(*resourceLoader.getTexture(ResourceLoader::BOARD_TEX)
		, *resourceLoader.getTexture(ResourceLoader::FIELD_TEX)
		, *resourceLoader.getTexture(ResourceLoader::FIELD_TEX));

    gameStates[0] = new GameState(players[0]->logicalMeepleBag, players[1]->logicalMeepleBag, logicalBoard);
    gameStates[1] = new GameState(players[1]->logicalMeepleBag, players[0]->logicalMeepleBag, logicalBoard);
    
	initMeeples();

	    

	background = new RBackground(resourceLoader,*_players[0],*_players[1]);

    //Game Menu Buttons:
        sf::Vector2f buttonOrigin(GAME_MENU_BUTTON_SIZE.x / 2.f, GAME_MENU_BUTTON_SIZE.y / 2.f);
	    restartButton.setTexture(resourceLoader.getTexture(ResourceLoader::RELOAD_BTN_TEX));
        restartButton.setFillColor(GAME_MENU_BUTTON_COLOR);
        restartButton.setSize(GAME_MENU_BUTTON_SIZE);
	    restartButton.setOrigin(buttonOrigin);
	    restartButton.setPosition(WINDOW_WIDTH_TO_CALCULATE / 2.f -150.f , WINDOW_HEIGHT_TO_CALCULATE / 2.f);

	    exitButton.setTexture(resourceLoader.getTexture(ResourceLoader::EXIT_BTN_TEX));
        exitButton.setFillColor(GAME_MENU_BUTTON_COLOR);
        exitButton.setSize(GAME_MENU_BUTTON_SIZE);
	    exitButton.setOrigin(buttonOrigin);
	    exitButton.setPosition(WINDOW_WIDTH_TO_CALCULATE / 2.f +150.f, WINDOW_HEIGHT_TO_CALCULATE / 2.f);
	
	    menuButton.setTexture(resourceLoader.getTexture(ResourceLoader::MENU_BTN_TEX));
        menuButton.setFillColor(GAME_MENU_BUTTON_COLOR);
        menuButton.setSize(GAME_MENU_BUTTON_SIZE);
	    menuButton.setOrigin(buttonOrigin);
	    menuButton.setPosition(WINDOW_WIDTH_TO_CALCULATE / 2.f, WINDOW_HEIGHT_TO_CALCULATE / 2.f);
    //Init Particle systems:
        particleSystem = new ParticleSystem(*resourceLoader.getTexture(ResourceLoader::PARTICLE_SPRITE), sf::Vector2u(4, 2));
    //FOR DUST-CLOUDS:
        dustBuilder = new ParticleBuilder({ 300, 300 }, { 5, 30 });
        dustBuilder->setSprites({ 2, 3 }, { 0, 1 })
            ->setPath({ 50, 150 }, { 290, 320 })
            ->setGravity(100, 90)
            ->setRotation()
            ->setFadeoutSpeed({ 300, 500 });       
    //FOR MOUSE-CLICKS:
        mouseCursorParticleBuilder = new ParticleBuilder({ 300, 300 }, { 5, 30 });
        mouseCursorParticleBuilder->setSprites({ 0, 3 }, { 0, 1 })
            ->setPath({ 5, 150 })
            ->setGravity(120, 90)
            ->setRotation({ 0, 600 }, { 0.1, 3.5 });    
    //End-Screen Animation:
        endScreenParticleBuilder = new ParticleBuilder({ 0, static_cast<float>(WINDOW_HEIGHT_TO_CALCULATE) }, { 5, 30 });
        endScreenParticleBuilder->setSprites({ 0, 1 }, { 0, 1 })
            ->setDynamicColor()
            ->setPath({ 10, 200 }, { 275, 350 })
            ->setGravity(30)
            ->setRotation({ 100, 600 }, { -1, 3 })
            ->setFadeoutSpeed({ 35, 65 });
}


Game::~Game(){
    delete endScreenParticleBuilder;
    delete mouseCursorParticleBuilder;
    delete dustBuilder;
    delete particleSystem;
    delete gameStates[1];
    delete gameStates[0];
	delete players[1]->logicalMeepleBag;
	delete players[0]->logicalMeepleBag;
	delete logicalBoard;
	delete board;				//	deletes all fields
	delete players[1]->rbag;	//	deletes all rendermeeples for p2
	delete players[0]->rbag;	//	deletes all rendermeeples for p1
}


void Game::reset(){
	players[0]->logicalMeepleBag->reset();
	players[1]->logicalMeepleBag->reset();

	logicalBoard->reset();
	players[0]->rbag->reset();
	players[1]->rbag->reset();

    for (uint8_t i = 0; i < 4; ++i){
        if (winningCombiRMeeples[i] == nullptr){
            break;
        }
        winningCombiRMeeples[i]->setGlow(nullptr);
        winningCombiRMeeples[i] = nullptr;
    }
    switchPlayers(); // TODO check this.. 
}



//Game Loop for one game, until there is a winner or the board is full
GameMenuDecision::Enum Game::runGame(){
	sf::Clock clock;
    float elapsedTime;
	
	meepleToSet = nullptr;
	hoveredMeeple = nullptr;
	rMeepleToSet = nullptr;
	posMeepleTo = { 42, 42 };
	dragMeeple = false;
    firstFrameOfState = false;

    LoopState lastLoopState = DISPLAY_END_SCREEN;   
	LoopState loopState = INIT_STATE;
    GameMenuDecision::Enum gameMenuDecision = GameMenuDecision::KEEP_PLAYING;
    
    while (window->isOpen() && gameMenuDecision == GameMenuDecision::KEEP_PLAYING){
        elapsedTime = clock.getElapsedTime().asSeconds();
	    float fps = 1.f / elapsedTime;
		clock.restart();
                
        if (rightMouseButtonPressed && rand()%5 == 0){
            mouseCursorParticleBuilder->setPosition(convertedMousePos);
            particleSystem->newParticleCloud(1, *mouseCursorParticleBuilder);
        }
        
		pollEvents();

		switch (loopState)
		{
		    case INIT_STATE:                                //todo das stimmt no ned ganz human iplayer und tc !!!!
			                                                loopState = players[activePlayerIndex]->type == Player::HUMAN ? HUMAN_SELECT_MEEPLE : TC_START_SELECT_MEEPLE;
			                                                break;
		    case I_PLAYER_SELECT_MEEPLE:					loopState = i_playerSelectMeeple();			                break;
		    case HUMAN_SELECT_MEEPLE:						loopState =  humanSelectMeeple();							break;
            case TC_START_SELECT_MEEPLE:					loopState = tcStartSelectMeeple();							break;
		    case TC_WAIT_FOR_SELECTED_MEEPLE:				loopState = tcWaitForSelectedMeeple();		                break;
		    case HIGHLIGHT_SELECTED_MEEPLE:					loopState = highlightSelectedMeeple(elapsedTime);			break;
		    case I_PLAYER_SELECT_MEEPLE_POSITION:			loopState = i_playerSelectMeeplePosition();                 break;
		    case HUMAN_SELECT_MEEPLE_POSITION:				loopState = humanSelectMeeplePosition();					break;
		    case TC_START_SELECT_MEEPLE_POSITION:			loopState = tcStartSelectMeeplePosition();					break;
		    case TC_WAIT_FOR_SELECTED_MEEPLE_POSITION:		loopState = tcWaitForSelectedMeeplePosition(); 	            break;
		    case MOVE_MEEPLE_TO_SELECTED_POSITION:			loopState = moveMeepleToSelectedPosition(elapsedTime);		break;
		    case CHECK_END_CONDITION:						loopState = checkEndCondition();							break;
		    case DISPLAY_END_SCREEN:			            gameMenuDecision = displayEndscreen(elapsedTime);			break;
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

		//window->draw(text);
             
		sort(meeplesToDrawAndSort.begin(), meeplesToDrawAndSort.end(), [](RMeeple* a, RMeeple* b){return a->getYPos() < b->getYPos(); });
		for (std::vector<RMeeple*>::iterator it = meeplesToDrawAndSort.begin(); it != meeplesToDrawAndSort.end(); ++it){
			(*it)->draw(*window);
		}

		particleSystem->update(elapsedTime);
		particleSystem->draw(*window);

        if (loopState == DISPLAY_END_SCREEN){
			window->draw(exitButton);
			window->draw(menuButton);
			window->draw(restartButton);
		}
	
		window->display();


        if (lastLoopState != loopState){
            firstFrameOfState = true;
            lastLoopState = loopState;
        }else{
            firstFrameOfState = false;
        }
	}
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

//return meeple pos and then set i
void Game::initMeeples(){
    uint8_t i;
	for (i = 0; i < 16; ++i){

		unsigned int meepleIndex;
		unsigned int bagInd = 0;
		if (i < 8){
			meepleIndex = i;
		}
		else{
			meepleIndex = i - 8;
			bagInd = 1;
		}
		const Meeple* meeple = players[bagInd]->logicalMeepleBag->getMeeple(meepleIndex);

		RMeeple* rmeeple = new RMeeple(*meeple,
			*resourceLoader->getTexture(ResourceLoader::MEEPLE_SPRITE),
			*resourceLoader->getTexture(ResourceLoader::MEEPLE_GLOW_SPRITE));

		meeplesToDrawAndSort.push_back(rmeeple);
		players[bagInd]->rbag->addRMeeple(rmeeple);
	}
    for (i = 0; i < 4; i++){
        winningCombiRMeeples[i] = nullptr;
    }
}






void Game::pollEvents(){
	pressedLeftMouse = false;
	releasedLeftMouse = false;

	sf::Event event;
	sf::Vector2i mousepos = sf::Mouse::getPosition(*window);
	convertedMousePos = window->mapPixelToCoords(mousepos);
	while (window->pollEvent(event)){
		switch (event.type){
			case sf::Event::MouseButtonPressed:
                switch (event.mouseButton.button){
                    case sf::Mouse::Left:
                        pressedLeftMouse = true;
                        break;
                    case sf::Mouse::Right:
                        rightMouseButtonPressed = true;
						background->setLeftWindow(true);
						background->setRightWindow(true);
                        break;
                    case sf::Mouse::Middle:
                    case sf::Mouse::XButton1:
                    case sf::Mouse::XButton2:
                    case sf::Mouse::ButtonCount:
                    default: break;
                }
                break;

			case sf::Event::MouseButtonReleased:
				switch (event.mouseButton.button){
				    case sf::Mouse::Left:
					    releasedLeftMouse = true;
					    break;
                    case sf::Mouse::Right:
                        rightMouseButtonPressed = false;
						background->setLeftWindow(false);
						background->setRightWindow(false);
                        break;
				    case sf::Mouse::Middle: 
				    case sf::Mouse::XButton1: 
				    case sf::Mouse::XButton2: 
				    case sf::Mouse::ButtonCount: 
				    default: break;
				}
				break;

			case sf::Event::MouseMoved:
				convertedMousePos = window->mapPixelToCoords(mousepos);
				break;

			case sf::Event::Resized:
				handleResizeWindowEvent(window);
				break;

			case sf::Event::Closed:
				window->close();
				break;

			default:
				break;
		}
	}
}


void Game::switchPlayers(){
	++activePlayerIndex;
	activePlayerIndex %= 2;
}

Game::LoopState Game::i_playerSelectMeeple(){
	assert(players[activePlayerIndex]->type == Player::I_PLAYER);
	meepleToSet = &players[activePlayerIndex]->player->selectOpponentsMeeple(*gameStates[activePlayerIndex]);
	return HIGHLIGHT_SELECTED_MEEPLE;
}

Game::LoopState Game::humanSelectMeeple(){
    assert(players[activePlayerIndex]->type == Player::HUMAN);

    if (releasedLeftMouse){
        rMeepleToSet = players[(activePlayerIndex + 1) % 2]->rbag->getRMeepleAtPosition(convertedMousePos);
        if (rMeepleToSet != nullptr){
            rMeepleToSet->setGlow(&SELECTED_MEEPLE_GLOW_COLOR);
            meepleToSet = rMeepleToSet->getLogicalMeeple();

            switchPlayers();
            hoveredMeeple = nullptr;
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
    hoveredMeeple = players[(activePlayerIndex + 1) % 2]->rbag->getRMeepleAtPosition(convertedMousePos);
    if (hoveredMeeple != nullptr){
        hoveredMeeple->setGlow(&HOVERED_MEEPLE_GLOW_COLOR);
    }
    return HUMAN_SELECT_MEEPLE;

}

Game::LoopState Game::tcStartSelectMeeple(){
	assert(players[activePlayerIndex]->type == Player::TC);
	players[activePlayerIndex]->controller->run_selectOpponentsMeeple(*gameStates[activePlayerIndex]);
	return TC_WAIT_FOR_SELECTED_MEEPLE;
}

Game::LoopState Game::tcWaitForSelectedMeeple(){
	assert(players[activePlayerIndex]->type == Player::TC);
	if (!players[activePlayerIndex]->controller->isResultAvailable()){
		return TC_WAIT_FOR_SELECTED_MEEPLE;
	}
	meepleToSet = &players[activePlayerIndex]->controller->getOpponentsMeeple();
	return HIGHLIGHT_SELECTED_MEEPLE;
}

Game::LoopState Game::highlightSelectedMeeple(float elapsedTime){
	assert(players[activePlayerIndex]->type == Player::TC);
	assert(meepleToSet != nullptr);

    if (firstFrameOfState){        
        remainingThinkTime = players[activePlayerIndex]->meepleChoosingThinkTime.get();
    }
    if (remainingThinkTime > 0){                //Time, before the AI starts moving the meeple
        remainingThinkTime -= elapsedTime;
        return HIGHLIGHT_SELECTED_MEEPLE;
    }
    
	switchPlayers();

	rMeepleToSet = players[activePlayerIndex]->rbag->isPassedMeepleInUnused(meepleToSet);
	assert(rMeepleToSet != nullptr);
	rMeepleToSet->setGlow(&SELECTED_MEEPLE_GLOW_COLOR);
	//highlight meeple (with glow animation? light -> heavy glow)
	//search meeple in meeplebag  -> glow

	//===todo auf 
	return players[activePlayerIndex]->type == Player::HUMAN ? HUMAN_SELECT_MEEPLE_POSITION : TC_START_SELECT_MEEPLE_POSITION;

}

Game::LoopState Game::i_playerSelectMeeplePosition(){
	assert(players[activePlayerIndex]->type == Player::I_PLAYER);
	posMeepleTo = players[activePlayerIndex]->player->selectMeeplePosition(*gameStates[activePlayerIndex], *meepleToSet);
	return MOVE_MEEPLE_TO_SELECTED_POSITION;
}

Game::LoopState Game::humanSelectMeeplePosition(){
	assert(players[activePlayerIndex]->type == Player::HUMAN);
	assert(rMeepleToSet != nullptr);
	//clicked meeple -> start to drag
	if (pressedLeftMouse && rMeepleToSet->containsPosition(convertedMousePos)){
		lastValidPosition = rMeepleToSet->getPosition();
		mousePosRelativeToMeepleBoundary = rMeepleToSet->getMousePosRelativeToMeepleBoundary(convertedMousePos);
		dragMeeple = true;
	}

	if (dragMeeple){ // todo checken ob !releaseleftmous braucht
		sf::Vector2f test(convertedMousePos.x - mousePosRelativeToMeepleBoundary.x, convertedMousePos.y - mousePosRelativeToMeepleBoundary.y);
		rMeepleToSet->setPosition(test);
		sf::Vector2f lookupPos = rMeepleToSet->getGlobalOrigin();
		board->setHoveredField(board->getBoardPosForPosititon(lookupPos));
	}

	if (releasedLeftMouse && dragMeeple){
		dragMeeple = false;
		sf::Vector2f lookupPos = rMeepleToSet->getGlobalOrigin();
		BoardPos pos = board->getBoardPosForPosititon(lookupPos);
		if ((pos.x < 4 && pos.y < 4) && logicalBoard->isFieldEmpty(pos)){
            rMeepleToSet->setPosition(board->getFieldCoords(pos));                       
			players[activePlayerIndex]->rbag->changeRMeepleToUsed(*rMeepleToSet);

			Meeple* placeMe = players[activePlayerIndex]->logicalMeepleBag->removeMeeple(*meepleToSet);
			logicalBoard->setMeeple(pos, *placeMe);
			rMeepleToSet->setGlow(nullptr);
			rMeepleToSet = nullptr;
			board->setHoveredField({ 42, 42 });

            createMeepleDust(board->getFieldGlobalBounds(pos));

			return CHECK_END_CONDITION;
		}
		rMeepleToSet->setPosition(lastValidPosition);
	}
	return HUMAN_SELECT_MEEPLE_POSITION;
}

Game::LoopState Game::tcStartSelectMeeplePosition(){
	assert(players[activePlayerIndex]->type == Player::TC);
	players[activePlayerIndex]->controller->run_selectMeeplePosition(*gameStates[activePlayerIndex], *meepleToSet);
	return TC_WAIT_FOR_SELECTED_MEEPLE_POSITION;
}

Game::LoopState Game::tcWaitForSelectedMeeplePosition(){
	assert(players[activePlayerIndex]->type == Player::TC);
	if (!players[activePlayerIndex]->controller->isResultAvailable()){
		return TC_WAIT_FOR_SELECTED_MEEPLE_POSITION;
	}
	posMeepleTo = players[activePlayerIndex]->controller->getMeeplePosition();
	return MOVE_MEEPLE_TO_SELECTED_POSITION;
}

Game::LoopState Game::moveMeepleToSelectedPosition(float elapsedTime){
    assert(players[activePlayerIndex]->type == Player::TC || players[activePlayerIndex]->type == Player::I_PLAYER);
	assert(rMeepleToSet != nullptr);
	assert(posMeepleTo.x < 4 && posMeepleTo.y < 4);

    if (firstFrameOfState){
        initialPosition = rMeepleToSet->getPosition();
        targetPosition = board->getFieldCoords(posMeepleTo);
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

    rMeepleToSet->setPosition(position);
	
    if (moveMeepleAnimationProgress >= 1){
        
        rMeepleToSet->setGlow(nullptr);

        //todo: inconsistent - we own the logicalboard, but we have to access the logicalBag via public membership
        Meeple* placeMe = players[activePlayerIndex]->logicalMeepleBag->removeMeeple(*meepleToSet); //Remove the meeple from the bag
        players[activePlayerIndex]->rbag->changeRMeepleToUsed(*rMeepleToSet);
        logicalBoard->setMeeple(posMeepleTo, *placeMe);

        rMeepleToSet->setPosition(targetPosition);

        createMeepleDust(board->getFieldGlobalBounds(posMeepleTo));
        return CHECK_END_CONDITION;
    }

	return MOVE_MEEPLE_TO_SELECTED_POSITION;
}

Game::LoopState Game::checkEndCondition(){
	#if PRINT_BOARD_TO_CONSOLE
		std::cout << "Player " << activePlayerIndex + 1 << " chose meeple \"" << meepleToSet->toString() << '\"' << std::endl;
		std::cout << "Player " << (activePlayerIndex + 1) % 2 + 1 << " sets meeple to " << meepleToSet->toString() << std::endl;
		logicalBoard->print(std::cout);
	#endif
	#if PRINT_WINNER_PER_ROUND
		std::cout << "Player " << activePlayerIndex + 1 << " wins!" << std::endl;
	#endif

	meepleToSet = nullptr;
	rMeepleToSet = nullptr;

	const WinCombination* combi = logicalBoard->checkWinSituation();
	if (combi != nullptr){
		for (uint8_t i = 0; i < 4; ++i){
			RMeeple* temp = players[activePlayerIndex]->rbag->isPassedMeepleInUsed(combi->meeples[i]);
			if (temp == nullptr){
				winningCombiRMeeples[i] = players[(activePlayerIndex + 1) % 2]->rbag->isPassedMeepleInUsed(combi->meeples[i]);
			}else{
				winningCombiRMeeples[i] = temp;
			}
			assert(winningCombiRMeeples[i] != nullptr);
		}
		return DISPLAY_END_SCREEN;
	}
	if (activePlayerIndex == 1 && logicalBoard->isFull()){
	    #if PRINT_WINNER_PER_ROUND
			    std::cout << "Tie! There is no winner." << std::endl;
	    #endif
		return DISPLAY_END_SCREEN;
	}
	return INIT_STATE;
}

GameMenuDecision::Enum Game::displayEndscreen(float elapsedTime){
	assert(winningCombiRMeeples[0] != nullptr && winningCombiRMeeples[1] != nullptr && winningCombiRMeeples[2] != nullptr && winningCombiRMeeples[3] != nullptr);

    if (firstFrameOfState || rand() % 100 < 50){
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
            colorAnimations[i].init(4, i*20);
        }        
    }

    //Animate rainbow-colors on the meeples, which were responsible for the winning-combination:  
        sf::Color color;
		for (uint8_t i = 0; i < 4; ++i){
            colorAnimations[i].animate(elapsedTime);
            color = colorAnimations[i].getColor();
            winningCombiRMeeples[i]->setGlow(&color);
		}

	if (hoveredButtonPtr != nullptr){
		hoveredButtonPtr->setFillColor(GAME_MENU_BUTTON_COLOR);
		hoveredButtonPtr = nullptr;
	}
	if (restartButton.getGlobalBounds().contains(convertedMousePos)){
        restartButton.setFillColor(GAME_MENU_BUTTON_HOVER_COLOR);
		hoveredButtonPtr = &restartButton;
	}else
	if (exitButton.getGlobalBounds().contains(convertedMousePos)){
        exitButton.setFillColor(GAME_MENU_BUTTON_HOVER_COLOR);
		hoveredButtonPtr = &exitButton;
	}else
	if (menuButton.getGlobalBounds().contains(convertedMousePos)){
        menuButton.setFillColor(GAME_MENU_BUTTON_HOVER_COLOR);
		hoveredButtonPtr = &menuButton;
	}

	if (releasedLeftMouse && restartButton.getGlobalBounds().contains(convertedMousePos)){
        return GameMenuDecision::REPLAY;
	}
	if (releasedLeftMouse && exitButton.getGlobalBounds().contains(convertedMousePos)){
        return GameMenuDecision::EXIT_GAME;
	}
	if (releasedLeftMouse && menuButton.getGlobalBounds().contains(convertedMousePos)){
        return GameMenuDecision::BACK_TO_MENU;
	}
    return GameMenuDecision::KEEP_PLAYING;      //stay in the game loop until the user presses a button
}
