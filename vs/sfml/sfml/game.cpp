#include "Game.h"
#include "Meeple.h"
#include "MeepleBag.h"

#include "Board.h"
#include "RBoard.h"

#include "I_Player.h"
#include "config.h"
#include "helper.h"

#include <SFML/System.hpp>
#include <SFML/System/Clock.hpp>

#include "RMeeple.h"

#include "ThreadController.h"

#include <vector>

#include <assert.h>
#include <string>
#include <iostream>

#include <math.h>


using namespace std;


//TODO verschieben???
//const unsigned int BOARD_X_OFFSET = 675;
//const unsigned int BOARD_Y_OFFSET = 150;
//
//const unsigned int BOARD_X_SPACEING = 240;
//const unsigned int BOARD_Y_SPACEING = 75;
//
//const unsigned int MEEPLE_SNAP_X = 22;
//const unsigned int MEEPLE_SNAP_Y = 65;
//
//const unsigned int MEEPLE_SNAP_OFFSET_X = 12;
//const unsigned int MEEPLE_SNAP_OFFSET_Y = 30;

const sf::Color STANDARD_GLOW = sf::Color::Yellow;
const sf::Color SELECTED_GLOW = sf::Color::Red;
//TODO players[] mit meeplebags in ein struct um verwechselung zu vermeiden



enum LoopState{
	INIT_STATE,
	//Select Meeple for opponent
	I_PLAYER_SELECT_MEEPLE, 
	HUMAN_SELECT_MEEPLE,
	TC_START_SELECT_MEEPLE, TC_WAIT_FOR_SELECTED_MEEPLE, HIGHLIGHT_SELECTED_MEEPLE,
	//Select Meeple position
	I_PLAYER_SELECT_MEEPLE_POSITION,
	HUMAN_SELECT_MEEPLE_POSITION,
	TC_START_SELECT_MEEPLE_POSITION, TC_WAIT_FOR_SELECTED_MEEPLE_POSITION, MOVE_MEEPLE_TO_SELECTED_POSITION,
	//check win /tie 
	CHECK_END_CONDITION, DISPLAY_END_SCREEN
};





Game::Game(sf::RenderWindow& window,Player& player1,  Player& player2) : window(&window) {
    assert(&player1 != &player2);   //hehehe, that won't crash this game

	players[0] = &player1;
	players[0]->logicalMeepleBag = new MeepleBag(MeepleColor::WHITE);
	
	players[1] = &player2;
	players[1]->logicalMeepleBag = new MeepleBag(MeepleColor::BLACK);
	logicalBoard = new Board();

	loadTextures();
	

	board = new RBoard(boardTexture, fieldTexture, fieldTextureOccupied);

	gameStatePlayer[0] = { players[0]->logicalMeepleBag, players[1]->logicalMeepleBag, logicalBoard };
	gameStatePlayer[1] = { players[1]->logicalMeepleBag, players[0]->logicalMeepleBag, logicalBoard };

	//setUp();
	initMeeples();
}


Game::~Game(){
	delete players[1]->logicalMeepleBag;
	delete players[0]->logicalMeepleBag;
	delete logicalBoard;
	delete board;

	/*for (vector<RField*>::iterator it = fields.begin(); it != fields.end(); ++it){
		delete *it;
	}*/

	for (uint8_t i = 0; i < 2; ++i){
		for (vector<RMeeple*>::iterator it = players[i]->rMeeples.begin(); it != players[i]->rMeeples.end(); ++it){
			delete *it;
		}
		//players[i]->rMeeples.clear();
		for (vector<RMeeple*>::iterator it = players[i]->usedRMeeples.begin(); it != players[i]->usedRMeeples.end(); ++it){
			delete *it;
		}
		//players[i]->usedRMeeples.clear();
	}
	//fields.clear();
}


void Game::reset(){
	players[0]->logicalMeepleBag->reset();
	players[1]->logicalMeepleBag->reset();

	logicalBoard->reset();
    
	//players1->reset();
 //   player2->reset();
	//TODO reset players.. inkl ifs.. 
	//players[0].controller->run_resetPlayer()
}




//Game Loop for one game, until there is a winner or the board is full
GameWinner::Enum Game::runGame(){
	sf::Clock clock;
	//clock.restart();

	const Meeple* meepleToSet = nullptr;
	rMeepleToSet = nullptr;
	BoardPos posMeepleTo;
	bool dragMeeple = false;


	//uint8_t activePlayerIndex = 0; //switch player in control here

	LoopState loopState = INIT_STATE;

	while (window->isOpen()){

	    float fps = 1 / clock.getElapsedTime().asSeconds();
		clock.restart();


		pollEvents();

		switch (loopState)
		{
		case INIT_STATE:
			//todo das stimmt no ned ganz human iplayer und tc !!!!
			loopState = players[activePlayerIndex]->type == Player::HUMAN ? HUMAN_SELECT_MEEPLE : TC_START_SELECT_MEEPLE;
				//goto ?
			break;

		case I_PLAYER_SELECT_MEEPLE:
			assert(players[activePlayerIndex]->type == Player::I_PLAYER);
			meepleToSet = &players[activePlayerIndex]->player->selectOpponentsMeeple(gameStatePlayer[activePlayerIndex]);
			loopState = HIGHLIGHT_SELECTED_MEEPLE;
			//todo: goto??
			break;

		case HUMAN_SELECT_MEEPLE:
			assert(players[activePlayerIndex]->type == Player::HUMAN);
			//human code
			{
				if (releasedLeftMouse){
					for (vector<RMeeple*>::iterator it = players[(activePlayerIndex + 1) % 2]->rMeeples.begin(); it != players[(activePlayerIndex + 1) % 2]->rMeeples.end(); ++it){
						if ((*it)->containsPosition(convertedMousePos)){
							(*it)->setGlow(&SELECTED_GLOW);

							rMeepleToSet = *it;
							meepleToSet = rMeepleToSet->getLogicalMeeple();

							switchPlayers();
							loopState = (players[activePlayerIndex]->type == Player::TC) ? TC_START_SELECT_MEEPLE_POSITION : I_PLAYER_SELECT_MEEPLE_POSITION;
							break;
						}
					}
				}
				for (vector<RMeeple*>::iterator it = players[(activePlayerIndex + 1) % 2]->rMeeples.begin(); it != players[(activePlayerIndex + 1) % 2]->rMeeples.end(); ++it){
					if ((*it)->containsPosition(convertedMousePos)){
						(*it)->setGlow(&STANDARD_GLOW);
					}
					else{
						(*it)->setGlow(nullptr);
					}
				}
				break;
			}
		case TC_START_SELECT_MEEPLE:
			assert(players[activePlayerIndex]->type == Player::TC);
			players[activePlayerIndex]->controller->run_selectOpponentsMeeple(gameStatePlayer[activePlayerIndex]);
			loopState = TC_WAIT_FOR_SELECTED_MEEPLE;
			break;

		case TC_WAIT_FOR_SELECTED_MEEPLE:
			assert(players[activePlayerIndex]->type == Player::TC);
			if (!players[activePlayerIndex]->controller->isResultAvailable()){
				break;
			}
			meepleToSet = &players[activePlayerIndex]->controller->getOpponentsMeeple();
			
			loopState = HIGHLIGHT_SELECTED_MEEPLE;
			//intentional fall through

		case HIGHLIGHT_SELECTED_MEEPLE:
			assert(players[activePlayerIndex]->type == Player::TC);
			
			switchPlayers(); // ohoh überprüfen ob eh ein switch angebracht ist bzw der state richtig gecallt wird!!

			for (vector<RMeeple*>::iterator it = players[activePlayerIndex]->rMeeples.begin(); it != players[activePlayerIndex]->rMeeples.end(); ++it){
				if ((*it)->representsPassedMeeple(meepleToSet)){
					rMeepleToSet = *it;
					break;
				}
			}
			assert(rMeepleToSet != nullptr);
			rMeepleToSet->setGlow(&SELECTED_GLOW);
			//rMeepleToSet->selectThisMeeple();
			//highlight meeple (with glow animation? light -> heavy glow)
			//search meeple in meeplebag  -> glow
			
			//todo auf 
			loopState = players[activePlayerIndex ]->type == Player::HUMAN ? HUMAN_SELECT_MEEPLE_POSITION : TC_START_SELECT_MEEPLE_POSITION;
			
			break;

		case I_PLAYER_SELECT_MEEPLE_POSITION:
			assert(players[activePlayerIndex]->type == Player::I_PLAYER);
			posMeepleTo = players[activePlayerIndex]->player->selectMeeplePosition(gameStatePlayer[activePlayerIndex], *meepleToSet);
			loopState = MOVE_MEEPLE_TO_SELECTED_POSITION;
			//todo: goto??
			break;

		case HUMAN_SELECT_MEEPLE_POSITION:
			assert(players[activePlayerIndex]->type == Player::HUMAN);
			assert(rMeepleToSet != nullptr);
			//human code
			//move my fucking meeple :)
			if (pressedLeftMouse && rMeepleToSet->containsPosition(convertedMousePos)){
				lastValidPosition = rMeepleToSet->getPosition();
				mousePosRelativeToMeepleBoundary = rMeepleToSet->getMousePosRelativeToMeepleBoundary(convertedMousePos);
				dragMeeple = true;
			}

			if (dragMeeple){ // todo checken ob !releaseleftmous braucht
				sf::Vector2f test(convertedMousePos.x - mousePosRelativeToMeepleBoundary.x, convertedMousePos.y - mousePosRelativeToMeepleBoundary.y);
				rMeepleToSet->setPosition(test);
				board->setHoveredField(board->getBoardPosForPosititon(convertedMousePos));
			}

			if (releasedLeftMouse && dragMeeple){
				dragMeeple = false;
				BoardPos pos;
				pos = board->getBoardPosForPosititon(convertedMousePos);
				if ((pos.x < 4 && pos.y < 4) && logicalBoard->isFieldEmpty(pos)){
					Meeple* placeMe = players[activePlayerIndex]->logicalMeepleBag->removeMeeple(*meepleToSet);
					logicalBoard->setMeeple(pos, *placeMe);
					rMeepleToSet->setGlow(nullptr);
					//todo einrasten machen snap meeeple
					loopState = CHECK_END_CONDITION;
				}
				else{
					rMeepleToSet->setPosition(lastValidPosition);
				}
			}
			break;

		case TC_START_SELECT_MEEPLE_POSITION:
			assert(players[activePlayerIndex]->type == Player::TC);
			players[activePlayerIndex]->controller->run_selectMeeplePosition(gameStatePlayer[activePlayerIndex], *meepleToSet);
			loopState = TC_WAIT_FOR_SELECTED_MEEPLE_POSITION;
			//fall through?
			break;

		case TC_WAIT_FOR_SELECTED_MEEPLE_POSITION:
			assert(players[activePlayerIndex]->type == Player::TC);
			if (!players[activePlayerIndex]->controller->isResultAvailable()){
				break;
			}
			posMeepleTo = players[activePlayerIndex]->controller->getMeeplePosition();
			

			loopState = MOVE_MEEPLE_TO_SELECTED_POSITION;
			//intentional fall through

		case MOVE_MEEPLE_TO_SELECTED_POSITION:
		{
			assert(players[activePlayerIndex]->type == Player::TC);
			assert(rMeepleToSet != nullptr);

			//uint8_t indexInVect = (posMeepleTo.x) +4 * posMeepleTo.y;

			//const sf::Vector2f* rMeeplePos = board->getFieldBounds(posMeepleTo);

			sf::FloatRect fieldBounds =  board->getFieldBounds(posMeepleTo);
			sf::Vector2f newPosition(
				fieldBounds.left,
				fieldBounds.top - 95.f); //todo magic num meepleheight

			rMeepleToSet->setPosition(newPosition);
			rMeepleToSet->setGlow(nullptr);

			Meeple* placeMe = players[activePlayerIndex]->logicalMeepleBag->removeMeeple(*meepleToSet);
			logicalBoard->setMeeple(posMeepleTo, *placeMe);

			players[activePlayerIndex]->rMeeples.erase(std::remove(players[activePlayerIndex]->rMeeples.begin(), 
														players[activePlayerIndex]->rMeeples.end(), rMeepleToSet));

			//todo sort rmeeple in y direction
			players[activePlayerIndex]->usedRMeeples.push_back(rMeepleToSet);
			//todo if pos fertig :)
			//switchPlayers();
			loopState = CHECK_END_CONDITION;
			break;
		}

		case CHECK_END_CONDITION:
				#if PRINT_BOARD_TO_CONSOLE
					cout << "Player " << activePlayerIndex + 1 << " chose meeple \"" << meepleToSet->toString() << '\"' << endl;
					cout << "Player " << (activePlayerIndex + 1) % 2 + 1 << " sets meeple to " << meepleToSet->toString() << endl;
					logicalBoard->print(cout);
				#endif
					meepleToSet = nullptr;
					rMeepleToSet = nullptr;
					if (logicalBoard->checkWinSituation()){    //player2 won
			        #if PRINT_WINNER_PER_ROUND
			            cout << "Player "<< activePlayerIndex+1 << " wins!" << endl;
			        #endif
					loopState = DISPLAY_END_SCREEN;
				}
					else if (activePlayerIndex == 1 && logicalBoard->isFull()){
			        #if PRINT_WINNER_PER_ROUND
			            cout << "Tie! There is no winner." << endl;
			        #endif
					loopState = DISPLAY_END_SCREEN;
				}
				else{
					//switchPlayers();
					loopState = INIT_STATE;
					break;
				}
				//intentional fall through

		case DISPLAY_END_SCREEN:

			// ein button für menü und ein button für restart

			// glow winning combination in rainbowcolor -> nyan cat -> nyan song


			//TODO player .. state resettten jakob? 
			break;

		}

		window->clear(sf::Color::White);
		board->draw(*window);
		for (uint8_t i = 0; i < 2; ++i){
			for (vector<RMeeple*>::iterator it = players[i]->rMeeples.begin(); it != players[i]->rMeeples.end(); ++it){
				(*it)->draw(*window);
			}
			for (vector<RMeeple*>::iterator it = players[i]->usedRMeeples.begin(); it != players[i]->usedRMeeples.end(); ++it){
				(*it)->draw(*window);
			}
		}
		std::string title("4Wins by Jakob M, Sebastian S and Simon D: @");
		title.append(std::to_string(fps));
		title.append(" fps");

		window->setTitle(title);
		window->display();
	}

	return GameWinner::PLAYER_1;
}


void Game::initMeeples(){
	//mylist.insert()
	//if (someIterator != someList.end()) {
		//someIterator++;
	//}
	//someList.insert(someIterator, someValue);

	cout << "init meeples" << endl;
	for (int i = 0; i < 16; ++i){

		unsigned int meepleIndex = 0;
		unsigned int bagInd = 0;
		if (i < 8){
			meepleIndex = i;
		}
		else{
			meepleIndex = i - 8;
			bagInd = 1;
		}
		const Meeple* meeple = players[bagInd]->logicalMeepleBag->getMeeple(meepleIndex);

		float xCoord = 20.f;
		float yCoord = 0.f;

		if (meeple->getColor() == MeepleColor::BLACK){
			xCoord += 1150.f;
		}

		if (meeple->getSize() == MeepleSize::SMALL){
			xCoord += 90.f;
		}
		
		if (meeple->getShape() == MeepleShape::SQUARE && meeple->getDetail() == MeepleDetail::NO_HOLE){
			yCoord = 10.f;
		}

		if (meeple->getShape() == MeepleShape::SQUARE && meeple->getDetail() == MeepleDetail::HOLE){
			yCoord = 180.f;
		}

		if (meeple->getShape() == MeepleShape::ROUND && meeple->getDetail() == MeepleDetail::NO_HOLE){
			yCoord = 350.f;
		}

		if (meeple->getShape() == MeepleShape::ROUND && meeple->getDetail() == MeepleDetail::HOLE){
			yCoord = 510.f;
		}

		sf::Vector2f initPos(xCoord, yCoord);
		RMeeple* rmeeple = new RMeeple(*meeple, meepleSprite, glowSprite, initPos);
		
		players[bagInd]->rMeeples.push_back(rmeeple);
	}
}


void Game::loadTextures(){
	if (!meepleSprite.loadFromFile(WORKING_DIR + "pencilStyle.png")){
		std::cerr << "couldn't load the texture: meepleSprites" << endl;
		// error...
	}
	if (!glowSprite.loadFromFile(WORKING_DIR + "glow.png")){
		std::cerr << "couldn't load the texture: glow" << endl;
		// error...
	}
	if (!boardTexture.loadFromFile(WORKING_DIR + "board.png")){ 
		std::cerr << "couldn't load the texture: board" << endl;
		/*error */ 
	}
	if (!fieldTexture.loadFromFile(WORKING_DIR + "holz.png")){
		std::cerr << "couldn't load the texture: holz" << endl;
		/*error */
	}
	if (!fieldTextureOccupied.loadFromFile(WORKING_DIR + "holz_occ.png")){
		std::cerr << "couldn't load the texture: holz_occ" << endl;
		/*error */
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
				if (event.mouseButton.button == sf::Mouse::Left){
					pressedLeftMouse = true;
				}
				break;

			case sf::Event::MouseButtonReleased:
				switch (event.mouseButton.button){
				case sf::Mouse::Left:
					releasedLeftMouse = true;
					break;

				default:
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

