#include "Game.h"
#include "Meeple.h"
#include "MeepleBag.h"

#include "Board.h"
#include "RBoard.h"
#include "RBag.h"

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




//using namespace std;


const sf::Color STANDARD_GLOW = sf::Color::Yellow;
const sf::Color SELECTED_GLOW = sf::Color::Red;


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





Game::Game(sf::RenderWindow& window,Player& player1,  Player& player2) : window(&window),drawEndScreen(false){
    assert(&player1 != &player2);   //hehehe, that won't crash this game

	players[0] = &player1;
	players[0]->logicalMeepleBag = new MeepleBag(MeepleColor::WHITE);
	players[0]->rbag = new RBag();

	players[1] = &player2;
	players[1]->logicalMeepleBag = new MeepleBag(MeepleColor::BLACK);
	players[1]->rbag = new RBag();

	logicalBoard = new Board();

	loadTextures();
	

	board = new RBoard(boardTexture, fieldTexture, fieldTextureOccupied);

    gameStates[0] = new GameState(players[0]->logicalMeepleBag, players[1]->logicalMeepleBag, logicalBoard);
    gameStates[1] = new GameState(players[1]->logicalMeepleBag, players[0]->logicalMeepleBag, logicalBoard);
    
	initMeeples();

	color4MGlow[0] = 0.1f;
	color4MGlow[1] = 0.25f;
	color4MGlow[2] = 0.35f;
	color4MGlow[3] = 0.45f;
}


Game::~Game(){
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
    
	//players1->reset();
 //   player2->reset();
	//TODO reset players.. inkl ifs.. 
	//players[0].controller->run_resetPlayer()
}



//Game Loop for one game, until there is a winner or the board is full
GameWinner::Enum Game::runGame(){
	sf::Clock clock;
	

	const Meeple* meepleToSet = nullptr;
	RMeeple* glowMeepleTmp = nullptr;
	rMeepleToSet = nullptr;
	BoardPos posMeepleTo{ 42, 42 };
	bool dragMeeple = false;

	LoopState loopState = INIT_STATE;

	while (window->isOpen()){

	    float fps = 1 / clock.getElapsedTime().asSeconds();
		clock.restart();

		text.setFont(font);
		text.setString("");
		text.setCharacterSize(50); // in pixels, not points!
		text.setColor(sf::Color::Black);
		//text.setStyle(sf::Text::Bold /*| sf::Text::Underlined*/);
		


		pollEvents();

		switch (loopState)
		{
		case INIT_STATE:
		{
			//todo das stimmt no ned ganz human iplayer und tc !!!!
			loopState = players[activePlayerIndex]->type == Player::HUMAN ? HUMAN_SELECT_MEEPLE : TC_START_SELECT_MEEPLE;
			//goto ?
			break;
		}
		case I_PLAYER_SELECT_MEEPLE:
		{
			assert(players[activePlayerIndex]->type == Player::I_PLAYER);
			meepleToSet = &players[activePlayerIndex]->player->selectOpponentsMeeple(*gameStates[activePlayerIndex]);
			loopState = HIGHLIGHT_SELECTED_MEEPLE;
			//todo: goto??
			break;
		}
		case HUMAN_SELECT_MEEPLE:
		{
			assert(players[activePlayerIndex]->type == Player::HUMAN);
			setString(std::string("Player " + std::to_string(activePlayerIndex + 1) + " choose Meeple"));

			if (releasedLeftMouse){
				rMeepleToSet = players[(activePlayerIndex + 1) % 2]->rbag->getRMeepleAtPosition(convertedMousePos);
				if (rMeepleToSet != nullptr)
				{
					rMeepleToSet->setGlow(&SELECTED_GLOW);
					meepleToSet = rMeepleToSet->getLogicalMeeple();

					switchPlayers();
					glowMeepleTmp = nullptr;
					if (players[activePlayerIndex]->type == Player::TC){
						loopState = TC_START_SELECT_MEEPLE_POSITION;
					}
					else if (players[activePlayerIndex]->type == Player::HUMAN) {
						loopState = HUMAN_SELECT_MEEPLE_POSITION;
					}
					else{
						loopState = I_PLAYER_SELECT_MEEPLE_POSITION;
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
			break;
		}
		case TC_START_SELECT_MEEPLE:
		{
			assert(players[activePlayerIndex]->type == Player::TC);
			players[activePlayerIndex]->controller->run_selectOpponentsMeeple(*gameStates[activePlayerIndex]);
			loopState = TC_WAIT_FOR_SELECTED_MEEPLE;
			break;
		}
		case TC_WAIT_FOR_SELECTED_MEEPLE:
		{
			assert(players[activePlayerIndex]->type == Player::TC);
			if (!players[activePlayerIndex]->controller->isResultAvailable()){
				break;
			}
			meepleToSet = &players[activePlayerIndex]->controller->getOpponentsMeeple();

			loopState = HIGHLIGHT_SELECTED_MEEPLE;
			//intentional fall through
		}
		case HIGHLIGHT_SELECTED_MEEPLE:
		{
			assert(players[activePlayerIndex]->type == Player::TC);

			switchPlayers();

			rMeepleToSet = players[activePlayerIndex]->rbag->isPassedMeepleInUnused(meepleToSet);
			assert(rMeepleToSet != nullptr);
			rMeepleToSet->setGlow(&SELECTED_GLOW);
			//highlight meeple (with glow animation? light -> heavy glow)
			//search meeple in meeplebag  -> glow

			//===todo auf 
			loopState = players[activePlayerIndex]->type == Player::HUMAN ? HUMAN_SELECT_MEEPLE_POSITION : TC_START_SELECT_MEEPLE_POSITION;
			break;
		}
		case I_PLAYER_SELECT_MEEPLE_POSITION:
		{
			assert(players[activePlayerIndex]->type == Player::I_PLAYER);
			posMeepleTo = players[activePlayerIndex]->player->selectMeeplePosition(*gameStates[activePlayerIndex], *meepleToSet);
			loopState = MOVE_MEEPLE_TO_SELECTED_POSITION;
			//todo: goto??
			break;
		}
		case HUMAN_SELECT_MEEPLE_POSITION:
		{
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

					players[activePlayerIndex]->rbag->changeRMeepleToUsed(*rMeepleToSet);

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
		}
		case TC_START_SELECT_MEEPLE_POSITION:
		{
			assert(players[activePlayerIndex]->type == Player::TC);
			players[activePlayerIndex]->controller->run_selectMeeplePosition(*gameStates[activePlayerIndex], *meepleToSet);
			loopState = TC_WAIT_FOR_SELECTED_MEEPLE_POSITION;
			//fall through?
			break;
		}
		case TC_WAIT_FOR_SELECTED_MEEPLE_POSITION:
		{
			assert(players[activePlayerIndex]->type == Player::TC);
			if (!players[activePlayerIndex]->controller->isResultAvailable()){
				break;
			}
			posMeepleTo = players[activePlayerIndex]->controller->getMeeplePosition();

			loopState = MOVE_MEEPLE_TO_SELECTED_POSITION;
			//intentional fall through
		}
		case MOVE_MEEPLE_TO_SELECTED_POSITION:
		{
			assert(players[activePlayerIndex]->type == Player::TC);
			assert(rMeepleToSet != nullptr);

			sf::FloatRect fieldBounds = board->getFieldBounds(posMeepleTo);
			sf::Vector2f newPosition(
				fieldBounds.left,
				fieldBounds.top - 95.f); //todo magic num meepleheight

			rMeepleToSet->setPosition(newPosition);
			rMeepleToSet->setGlow(nullptr);

			Meeple* placeMe = players[activePlayerIndex]->logicalMeepleBag->removeMeeple(*meepleToSet);
			logicalBoard->setMeeple(posMeepleTo, *placeMe);

			players[activePlayerIndex]->rbag->changeRMeepleToUsed(*rMeepleToSet);

			loopState = CHECK_END_CONDITION;
			break;
		}

		case CHECK_END_CONDITION:
		{
			#if PRINT_BOARD_TO_CONSOLE
			std::cout << "Player " << activePlayerIndex + 1 << " chose meeple \"" << meepleToSet->toString() << '\"' << std::endl;
			std::cout << "Player " << (activePlayerIndex + 1) % 2 + 1 << " sets meeple to " << meepleToSet->toString() << std::endl;
				logicalBoard->print(std::cout);
			#endif
			meepleToSet = nullptr;
			rMeepleToSet = nullptr;

			const WinCombination* combi = logicalBoard->checkWinSituation();
			if (combi != nullptr){    //player2 won
				#if PRINT_WINNER_PER_ROUND
				std::cout << "Player " << activePlayerIndex + 1 << " wins!" << std::endl;
				#endif
				loopState = DISPLAY_END_SCREEN;
				
				for (uint8_t i = 0; i < 4; ++i){
					RMeeple* temp = players[activePlayerIndex]->rbag->isPassedMeepleInUsed(combi->meeples[i]);
					if (temp == nullptr)
					{
						winningCombiRMeeples[i] = players[(activePlayerIndex + 1) % 2]->rbag->isPassedMeepleInUsed(combi->meeples[i]);
					} else
					{
						winningCombiRMeeples[i] = temp;
					}
					assert(winningCombiRMeeples[i] != nullptr);
				}
			}
			else if (activePlayerIndex == 1 && logicalBoard->isFull()){
				#if PRINT_WINNER_PER_ROUND
					std::cout << "Tie! There is no winner." << std::endl;
				#endif
				loopState = DISPLAY_END_SCREEN;
			}
			else{
				//switchPlayers();
				loopState = INIT_STATE;
				break;
			}
			//intentional fall through
		}
		case DISPLAY_END_SCREEN:
			assert(winningCombiRMeeples[0] != nullptr && winningCombiRMeeples[1] != nullptr &&winningCombiRMeeples[2] != nullptr &&winningCombiRMeeples[3] != nullptr);
			drawEndScreen = true;
			
			if (meepleGlowAnimationClock.getElapsedTime().asSeconds() > 0.03f){

				//color4MGlow[] = 0.f;
				for (uint8_t i = 0; i < 4; ++i){
				sf::Color c= rainbow(color4MGlow[i]);
					winningCombiRMeeples[i]->setGlow(&c);

					color4MGlow[i] += 0.01f;
					if (color4MGlow[i] > 1){
						color4MGlow[i] = 0;
					}
				}
				meepleGlowAnimationClock.restart();
			}
			
			// ein button für menü und ein button für wrestart

			// glow winning combination in rainbowcolor -> nyan cat -> nyan song



			//TODO player .. state resettten jakob? 
			break;

		}

		std::string title("4Wins by Jakob M., Sebastian S. and Simon D.   @");
		title.append(std::to_string(fps));
		title.append(" fps");
		window->setTitle(title);

		window->clear(sf::Color::White);
		
		board->draw(*window);

		players[0]->rbag->draw(*window);
		players[1]->rbag->draw(*window);

		window->draw(text);
		window->display();
	}

	return GameWinner::PLAYER_1;
}

sf::Color Game::rainbow(float progress) const
{
	/*convert to rainbow RGB*/
	float a = (1.f - progress) / 0.2f;
	uint8_t X = static_cast<uint8_t>(floor(a));
	float Y = floor(255 * (a - X));
	switch (X)
	{
	case 0:
		return sf::Color(255, static_cast<uint8_t>( Y), 0, 255);

	case 1:
		return sf::Color(static_cast<uint8_t>(255 - Y) , 255, 0, 255);

	case 2:
		return sf::Color(0, 255, static_cast<uint8_t>(Y), 255);

	case 3:
		return	sf::Color(0, static_cast<uint8_t>(255 - Y), 255, 255);

	case 4:
		return sf::Color(static_cast<uint8_t>(Y), 0, 255, 255);

	case 5:
		return sf::Color(255, 0, 255, 255);
	default:
		return sf::Color::Blue;
	}
}


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
		
		players[bagInd]->rbag->addRMeeple(rmeeple);
	}
}


void Game::loadTextures(){
	if (!meepleSprite.loadFromFile(WORKING_DIR + "pencilStyle.png")){
		std::cerr << "couldn't load the texture: meepleSprites" << std::endl;
		assert(false);
	}
	if (!glowSprite.loadFromFile(WORKING_DIR + "glow_grey.png")){
		std::cerr << "couldn't load the texture: glow" << std::endl;
		assert(false);
	}
	if (!boardTexture.loadFromFile(WORKING_DIR + "board.png")){ 
		std::cerr << "couldn't load the texture: board" << std::endl;
		assert(false);
	}
	if (!fieldTexture.loadFromFile(WORKING_DIR + "holz.png")){
		std::cerr << "couldn't load the texture: holz" << std::endl;
		assert(false);
	}
	if (!fieldTextureOccupied.loadFromFile(WORKING_DIR + "holz_occ.png")){
		std::cerr << "couldn't load the texture: holz_occ" << std::endl;
		assert(false);
	}

	if (!font.loadFromFile(WORKING_DIR + "Fonts/roboto/roboto-black.ttf"))
	{
		std::cerr << "Couldn't load the Font: roboto-black.ttf" << std::endl;
		assert(false);
	}
}


void Game::setString(std::string message)
{

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);

	text.setString(message);
	//text.setOrigin(0, 0);


	textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		25.0f);
	//window->mapCoordsToPixel( window->getSize())
	//text.getLocalBounds().width
	text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 0.0f));

	//float xPostext = (window->getSize().x / 2.f);
	//std::cout << "windowsize /2: " << xPostext << std::endl;
	//float test2 = (text.getGlobalBounds().width / 2);
	//std::cout << "test2 /2: " << test2 << std::endl;
	//text.setPosition(sf::Vector2f(xPostext + test2, 10.f));


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

