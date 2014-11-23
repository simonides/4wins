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


//
//const sf::Color STANDARD_GLOW = sf::Color::Yellow;
//const sf::Color SELECTED_GLOW = sf::Color::Red;


#define DUST_X_OFFSET -10     //X-Offset for producing dust when placing a meeple

Game::Game(sf::RenderWindow& window, Player* _players[2], ResourceLoader& resourceLoader)
	: window(&window), resourceLoader(&resourceLoader)
	, pressedLeftMouse(false)
	, releasedLeftMouse(false), rMeepleToSet(nullptr)
	, activePlayerIndex(0), STANDARD_GLOW(sf::Color::Yellow)
	, SELECTED_GLOW(sf::Color::Red), drawEndScreen(false)
	, hoveredButtonPtr(nullptr), buttonColor(sf::Color(119, 110, 0, 255))
    , dustBuilder(nullptr), endScreenParticleBuilder(nullptr)
    , rightMouseButtonPressed(false)
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

	    

	sf::Vector2f buttonSize(150.f, 150.f);
	sf::Vector2f buttonOrigin(75.f, 75.f);

	background = new RBackground(resourceLoader,*_players[0],*_players[1]);

	restartButton.setTexture(resourceLoader.getTexture(ResourceLoader::RELOAD_BTN_TEX));
	restartButton.setFillColor(buttonColor);
	restartButton.setSize(buttonSize);
	restartButton.setOrigin(buttonOrigin);
	restartButton.setPosition(WINDOW_WIDTH_TO_CALCULATE / 2.f -150.f , WINDOW_HEIGHT_TO_CALCULATE / 2.f);


	exitButton.setTexture(resourceLoader.getTexture(ResourceLoader::EXIT_BTN_TEX));
	exitButton.setFillColor(buttonColor);
	exitButton.setSize(buttonSize);
	exitButton.setOrigin(buttonOrigin);
	exitButton.setPosition(WINDOW_WIDTH_TO_CALCULATE / 2.f +150.f, WINDOW_HEIGHT_TO_CALCULATE / 2.f);
	
	menuButton.setTexture(resourceLoader.getTexture(ResourceLoader::MENU_BTN_TEX));
	menuButton.setFillColor(buttonColor);
	menuButton.setSize(buttonSize);
	menuButton.setOrigin(buttonOrigin);
	menuButton.setPosition(WINDOW_WIDTH_TO_CALCULATE / 2.f, WINDOW_HEIGHT_TO_CALCULATE / 2.f);

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
        winningCombiRMeeples[i]->setGlow(nullptr);
        winningCombiRMeeples[i] = nullptr;
    }
    switchPlayers(); // TODO check this.. 
}



//Game Loop for one game, until there is a winner or the board is full
GameReturn Game::runGame(){

	sf::Clock clock;
    float elapsedTime = 0;
	
	meepleToSet = nullptr;
	glowMeepleTmp = nullptr;
	rMeepleToSet = nullptr;
	posMeepleTo = { 42, 42 };
	dragMeeple = false;
    drawEndScreen = false;

	LoopState loopState = INIT_STATE;
    GameReturn gameReturn = KEEP_PLAYING;

    while (window->isOpen() && gameReturn == KEEP_PLAYING){
        elapsedTime = clock.getElapsedTime().asSeconds();
	    float fps = 1.f / elapsedTime;
		clock.restart();

		//text.setFont(font);
		//text.setString("");
		//text.setCharacterSize(50); // in pixels, not points!
		//text.setColor(sf::Color::Black);
		//text.setStyle(sf::Text::Bold /*| sf::Text::Underlined*/);
		
        
        if (rightMouseButtonPressed && rand()%5 == 0){
            mouseCursorParticleBuilder->setPosition(convertedMousePos);
            particleSystem->newParticleCloud(1, *mouseCursorParticleBuilder);
        }
        
		pollEvents();

		switch (loopState)
		{
		case INIT_STATE:
			//todo das stimmt no ned ganz human iplayer und tc !!!!
			loopState = players[activePlayerIndex]->type == Player::HUMAN ? HUMAN_SELECT_MEEPLE : TC_START_SELECT_MEEPLE;
			//goto ?
			break;

		case I_PLAYER_SELECT_MEEPLE:					loopState = i_playerSelectMeeple();	/*todo: goto?? */		break;

		case HUMAN_SELECT_MEEPLE:						loopState =  humanSelectMeeple();							break;

        case TC_START_SELECT_MEEPLE:					loopState = tcStartSelectMeeple();							break;

		case TC_WAIT_FOR_SELECTED_MEEPLE:				loopState = tcWaitForSelectedMeeple();/*todo: goto?? */		break;

		case HIGHLIGHT_SELECTED_MEEPLE:					loopState = highlightSelectedMeeple();						break;

		case I_PLAYER_SELECT_MEEPLE_POSITION:			loopState = i_playerSelectMeeplePosition();/*todo: goto??*/ break;

		case HUMAN_SELECT_MEEPLE_POSITION:				loopState = humanSelectMeeplePosition();					break;

		case TC_START_SELECT_MEEPLE_POSITION:			loopState = tcStartSelectMeeplePosition();					break;

		case TC_WAIT_FOR_SELECTED_MEEPLE_POSITION:		loopState = tcWaitForSelectedMeeplePosition(); /*goto?*/	break;

		case MOVE_MEEPLE_TO_SELECTED_POSITION:			loopState = MoveMeepleToSelectedPosition();					break;

		case CHECK_END_CONDITION:						loopState = checkEndCondition();							break;

		case DISPLAY_END_SCREEN:			            gameReturn = displayEndscreen(elapsedTime);					break;
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
             
		std::sort(meeplesToDrawAndSort.begin(), meeplesToDrawAndSort.end(), [](RMeeple* a, RMeeple* b){return a->getYPos() < b->getYPos(); });
		for (std::vector<RMeeple*>::iterator it = meeplesToDrawAndSort.begin(); it != meeplesToDrawAndSort.end(); ++it){
			(*it)->draw(*window);
		}

		particleSystem->update(elapsedTime);
		particleSystem->draw(*window);

		if (drawEndScreen)
		{
			window->draw(exitButton);
			window->draw(menuButton);
			window->draw(restartButton);
		}
	
		window->display();
	}
    return gameReturn;
}

void Game::createMeepleDust(sf::FloatRect fieldBounds){
    dustBuilder->setPosition({ fieldBounds.left - DUST_X_OFFSET, fieldBounds.top + fieldBounds.height / 2 })
               ->setPath({ 50, 150 }, { 220, 250 });  //left dust
    particleSystem->newParticleCloud(20, *dustBuilder);    
    dustBuilder->setPosition({ fieldBounds.left + fieldBounds.width + DUST_X_OFFSET, fieldBounds.top + fieldBounds.height / 2 })
               ->setPath({ 50, 150 }, { 290, 320 }); //right dust
    particleSystem->newParticleCloud(20, *dustBuilder);
    //particleSystem->newParticleCloud(20, *dustBuilder);
}

//return meeple pos and then set i
void Game::initMeeples(){
	//cout << "init meeples" << endl;
	for (int i = 0; i < 16; ++i){

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
			//setString(std::string("Player " + std::to_string(activePlayerIndex + 1) + " choose Meeple"));

			if (releasedLeftMouse){
				rMeepleToSet = players[(activePlayerIndex + 1) % 2]->rbag->getRMeepleAtPosition(convertedMousePos);
				if (rMeepleToSet != nullptr)
				{
					rMeepleToSet->setGlow(&SELECTED_GLOW);
					sf::Color yellow(sf::Color::Yellow);
					rMeepleToSet->setGlow(&yellow);
					meepleToSet = rMeepleToSet->getLogicalMeeple();

					switchPlayers();
					glowMeepleTmp = nullptr;
					if (players[activePlayerIndex]->type == Player::TC){
						return TC_START_SELECT_MEEPLE_POSITION;
					}
					else if (players[activePlayerIndex]->type == Player::HUMAN) {
						return HUMAN_SELECT_MEEPLE_POSITION;
					}
					else{
						return I_PLAYER_SELECT_MEEPLE_POSITION;
					}
				}
			}
			if (glowMeepleTmp != nullptr)
			{
				glowMeepleTmp->setGlow(nullptr);
			}
			glowMeepleTmp = players[(activePlayerIndex + 1) % 2]->rbag->getRMeepleAtPosition(convertedMousePos);
			if (glowMeepleTmp != nullptr)
			{
				glowMeepleTmp->setGlow(&STANDARD_GLOW);
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
Game::LoopState Game::highlightSelectedMeeple(){
	assert(players[activePlayerIndex]->type == Player::TC);
	assert(meepleToSet != nullptr);

	switchPlayers();

	rMeepleToSet = players[activePlayerIndex]->rbag->isPassedMeepleInUnused(meepleToSet);
	assert(rMeepleToSet != nullptr);
	rMeepleToSet->setGlow(&SELECTED_GLOW);
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

			sf::FloatRect fieldBounds = board->getFieldGlobalBounds(pos);
			sf::Vector2f newPosition(fieldBounds.left + fieldBounds.width / 2.f, fieldBounds.top + fieldBounds.height / 2.f);
			rMeepleToSet->setPosition(newPosition);
                       
			players[activePlayerIndex]->rbag->changeRMeepleToUsed(*rMeepleToSet);

			Meeple* placeMe = players[activePlayerIndex]->logicalMeepleBag->removeMeeple(*meepleToSet);
			logicalBoard->setMeeple(pos, *placeMe);
			rMeepleToSet->setGlow(nullptr);
			rMeepleToSet = nullptr;
			board->setHoveredField({ 42, 42 });

            createMeepleDust(fieldBounds);

			return CHECK_END_CONDITION;
		}
		else{
			rMeepleToSet->setPosition(lastValidPosition);
		}
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
Game::LoopState Game::MoveMeepleToSelectedPosition(){
	assert(players[activePlayerIndex]->type == Player::TC);
	assert(rMeepleToSet != nullptr);
	assert(posMeepleTo.x < 4 && posMeepleTo.y < 4);

	sf::FloatRect fieldBounds = board->getFieldGlobalBounds(posMeepleTo);
	sf::Vector2f newPosition(fieldBounds.left + fieldBounds.width / 2.f, fieldBounds.top + fieldBounds.height / 2.f);

	rMeepleToSet->setPosition(newPosition);
	rMeepleToSet->setGlow(nullptr);

	Meeple* placeMe = players[activePlayerIndex]->logicalMeepleBag->removeMeeple(*meepleToSet);
	logicalBoard->setMeeple(posMeepleTo, *placeMe);

	players[activePlayerIndex]->rbag->changeRMeepleToUsed(*rMeepleToSet);

    createMeepleDust(fieldBounds);

	return CHECK_END_CONDITION;

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
	if (combi != nullptr){    //player2 won
		for (uint8_t i = 0; i < 4; ++i){
			RMeeple* temp = players[activePlayerIndex]->rbag->isPassedMeepleInUsed(combi->meeples[i]);
			if (temp == nullptr)
			{
				winningCombiRMeeples[i] = players[(activePlayerIndex + 1) % 2]->rbag->isPassedMeepleInUsed(combi->meeples[i]);
			}
			else
			{
				winningCombiRMeeples[i] = temp;
			}
			assert(winningCombiRMeeples[i] != nullptr);
		}
		return DISPLAY_END_SCREEN;
	}
	else if (activePlayerIndex == 1 && logicalBoard->isFull()){
	#if PRINT_WINNER_PER_ROUND
			std::cout << "Tie! There is no winner." << std::endl;
	#endif
		return DISPLAY_END_SCREEN;
	}
	else{
		//switchPlayers();// if not switched the other player starts... 
		return INIT_STATE;
	}
}


GameReturn Game::displayEndscreen(float elapsedTime){
	assert(winningCombiRMeeples[0] != nullptr && winningCombiRMeeples[1] != nullptr &&winningCombiRMeeples[2] != nullptr &&winningCombiRMeeples[3] != nullptr);

	if (drawEndScreen != true || rand() % 100 < 50){
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
    if (drawEndScreen != true){
        for (uint8_t i = 0; i < 4; i++){
            colorAnimations[i].init(4, i*25);
        }        
    }
	drawEndScreen = true;
    //Animate rainbow-colors on the meeples, which were responsible for the winning-combination:  
        sf::Color color;
		for (uint8_t i = 0; i < 4; ++i){
            colorAnimations[i].animate(elapsedTime);
            color = colorAnimations[i].getColor();
            winningCombiRMeeples[i]->setGlow(&color);
		}


	if (hoveredButtonPtr != nullptr)
	{
		hoveredButtonPtr->setFillColor(buttonColor);
		hoveredButtonPtr = nullptr;
	}
	if (restartButton.getGlobalBounds().contains(convertedMousePos))
	{
		restartButton.setFillColor(sf::Color::Magenta);
		hoveredButtonPtr = &restartButton;
	}

	if (exitButton.getGlobalBounds().contains(convertedMousePos))
	{
		exitButton.setFillColor(sf::Color::Magenta);
		hoveredButtonPtr = &exitButton;
	}
	if (menuButton.getGlobalBounds().contains(convertedMousePos))
	{
		menuButton.setFillColor(sf::Color::Magenta);
		hoveredButtonPtr = &menuButton;
	}

	if (releasedLeftMouse && restartButton.getGlobalBounds().contains(convertedMousePos)){
		return REPLAY;
	}
	if (releasedLeftMouse && exitButton.getGlobalBounds().contains(convertedMousePos)){
		return EXIT_GAME;
	}
	if (releasedLeftMouse && menuButton.getGlobalBounds().contains(convertedMousePos)){
		return BACK_TO_MENU;
	}
	return KEEP_PLAYING;
}

//void Game::setString(std::string message)
//{
//	text.setString(message);
//
//	sf::FloatRect textRect = text.getLocalBounds();
//	text.setOrigin(textRect.left + textRect.width / 2.0f,
//		25.0f);
//	text.setPosition(sf::Vector2f(WINDOW_WIDTH_TO_CALCULATE / 2.0f, 35.0f));
//}