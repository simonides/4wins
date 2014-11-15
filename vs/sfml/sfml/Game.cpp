#include "Game.h"
#include "Meeple.h"
#include "MeepleBag.h"
#include "Board.h"
#include "I_Player.h"
#include "config.h"
#include "helper.h"
#include <SFML/System.hpp>

#include "RMeeple.h"

//#include <list>
#include <vector>
//#include <string>//eventuell wieder löschen

#include <assert.h>
#include <string>
#include <iostream>


using namespace std;

const unsigned int BOARD_X_OFFSET = 675;
const unsigned int BOARD_Y_OFFSET = 150;

const unsigned int BOARD_X_SPACEING = 240;
const unsigned int BOARD_Y_SPACEING = 75;


const unsigned int MEEPLE_SNAP_X = 22;
const unsigned int MEEPLE_SNAP_Y = 65;

const unsigned int MEEPLE_SNAP_OFFSET_X = 12;
const unsigned int MEEPLE_SNAP_OFFSET_Y = 30;

Game::Game(sf::RenderWindow& window,I_Player& player1, I_Player& player2) : window(&window), player1(&player1), player2(&player2) {
    assert(&player1 != &player2);   //hehehe, that won't crash this game

    bag[0] = new MeepleBag(MeepleColor::WHITE);
    bag[1] = new MeepleBag(MeepleColor::BLACK);

	gameStatePlayer1 = { bag[0], bag[1], &board };
    gameStatePlayer2 = { bag[1], bag[0], &board };
	
	setUp();
}


Game::~Game(){
    delete bag[1];
    delete bag[0];
	for (vector<sf::CircleShape*>::iterator it = fields.begin(); it != fields.end(); ++it){
		delete *it;
	}
	//fields.clear();
}


void Game::reset(){
    bag[0]->reset();
    bag[1]->reset();

	board.reset();
    
	player1->reset();
    player2->reset();
}

//sf::Clock clock;
//float lastTime = 0;
//while (Window.IsOpen())
//{
//	...
//	float currentTime = clock.Restart().AsSeconds();
//	float fps = 1.f / (currentTime - lastTime);
//	lastTime = currentTime;
//}
//

//Game Loop for one game, until there is a winner or the board is full
GameWinner::Enum Game::runGame(){
	//sf::Clock /*clock;
	//float lastTime */= 0;

	while (window->isOpen()){
		pollEvents();
		
		window->clear(sf::Color::White);

		window->draw(boardPanel);
		window->draw(leftPanel);
		window->draw(rightPanel);
		
		for (vector<sf::CircleShape*>::iterator it = fields.begin(); it != fields.end(); ++it){
			window->draw(**it);
		}

		for (vector<RMeeple*>::iterator it = meeples.begin(); it != meeples.end(); ++it){
			(*it)->draw(*window);
		}


		//float currentTime = clock.restart().asSeconds();
		//float fps = 1.f / (currentTime - lastTime);
		//lastTime = currentTime;

		//std::string title("test: ");
		//title.append();
		
		//window->setTitle(std::to_string(fps));
		window->display();
	}

    //for (;;){
    //    runGameCycle(player1, player2, gameStatePlayer1, gameStatePlayer2 ,0);
    //    if (board->checkWinSituation()){    //player2 won
    //        #if PRINT_WINNER_PER_ROUND
    //            cout << "Player 2 wins!" << endl;
    //        #endif
    //        return GameWinner::PLAYER_2;
    //    }

    //    runGameCycle(player2, player1, gameStatePlayer2, gameStatePlayer1, 1);
    //    if (board->checkWinSituation()){    //player1 won
    //        #if PRINT_WINNER_PER_ROUND
    //            cout << "Player 1 wins!" << endl;
    //        #endif
    //        return GameWinner::PLAYER_1;
    //    }

    //    if (board->isFull()){
    //        #if PRINT_WINNER_PER_ROUND
    //            cout << "Tie! There is no winner." << endl;
    //        #endif
    //        return GameWinner::TIE;
    //    }
    //}
	return GameWinner::PLAYER_1;
}


//a have round cycle, where a player chooses a meeple, and the other player sets it
void Game::runGameCycle(I_Player* player, I_Player* opponent, GameState& gameStateForPlayer, GameState& gameStateForOpponent, int playerNr){

    const Meeple& toSet = player->selectOpponentsMeeple(gameStateForPlayer);    //player selects a meeple
    Meeple* meeple = bag[(playerNr + 1) % 2]->removeMeeple(toSet);              //remove meeple from opponent's bag          
    

	//for (vector<RMeeple*>::iterator it = meeples.begin(); it != meeples.end(); ++it){
	//	(*it)->draw(*window);
	//}

    BoardPos pos = opponent->selectMeeplePosition(gameStateForOpponent, *meeple); //select a position
    assert(pos.x < 4 && pos.y < 4);
    board.setMeeple(pos, *meeple);                                             //set the meeple
    
    //Debug:
    #if PRINT_BOARD_TO_CONSOLE
        cout << "Player " << playerNr + 1 << " chose meeple \"" << toSet.toString() << '\"' << endl;
        cout << "Player " << (playerNr + 1) % 2 + 1 << " sets meeple to (" << static_cast<int>(pos.x) << 'x' << static_cast<int>(pos.y) << ')' << endl;
        board->print(cout);
    #endif
    #if STEP_BY_STEP
        cin.ignore();
    #endif
}

void Game::setUp(){
	loadTextures();
	leftPanel.setSize(sf::Vector2f(200.f, 690.f));
	leftPanel.setPosition(sf::Vector2f(0.f, 0.f));
	leftPanel.setFillColor(sf::Color::Green);

	rightPanel.setSize(sf::Vector2f(200.f, 690.f));
	rightPanel.setPosition(sf::Vector2f(1150.f, 0.f));
	rightPanel.setFillColor(sf::Color::Magenta);


	
	
	boardPanel.setSize(sf::Vector2f(620.f*1.615f, 620.f)); //TODO make constants or calc
	boardPanel.setFillColor(sf::Color::White);
	boardPanel.setPosition(sf::Vector2f(180, 70));							//TODO calc
	boardPanel.setTexture(&boardTexture);
	
	initFields();
	initMeeples();
}

void Game::initFields(){
	//fields = new sf::CircleShape[16]();

	int counter = 0;
	for (unsigned int y = 0; y < 7; ++y){
		//cout << "counter in loop: " << counter << endl;
		int times = (y + 1) > 3 ? 7 - y : y + 1;
		//cout << "times: " << times << endl;
		float calcX = BOARD_X_OFFSET - ((times - 1) / 2.f *BOARD_X_SPACEING);
		for (int x = 0; x < times; ++x){
			sf::CircleShape* field = new sf::CircleShape();
			field->setOrigin(sf::Vector2f(40.f, 40.f));
			field->setFillColor(sf::Color::Black);
			field->setRadius(40);
			field->setPointCount(6);
			field->setPosition(sf::Vector2f(calcX, static_cast<float>(y * BOARD_Y_SPACEING + BOARD_Y_OFFSET)));
			calcX += BOARD_X_SPACEING;
			fields.push_back(field);
			++counter;
		}
	}
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

		const Meeple* meeple = bag[bagInd]->getMeeple(meepleIndex);

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

		sf::IntRect texRec = getTextRectForPos(*meeple);
		sf::IntRect glowTexRec = getGlowTextRectForPos(*meeple);
		//cout << endl;
		sf::Vector2f initPos(xCoord, yCoord);
		RMeeple* rmeeple = new RMeeple(*meeple, meepleSprite, texRec, glowSprite, glowTexRec, initPos);
		
		meeples.push_back(rmeeple);
		
	}
}

sf::IntRect Game::getTextRectForPos(const Meeple& meeple){

	unsigned int sizeX = meepleSprite.getSize().x;
	unsigned int sizeY = meepleSprite.getSize().y;
	
	unsigned int row = 0;
	unsigned int column = 0;
	if (meeple.getColor() == MeepleColor::BLACK){
		row = 1;
	}
	if (meeple.getShape() == MeepleShape::ROUND){
		column += 4;
	}
	if (meeple.getSize() == MeepleSize::SMALL){
		column += 1;
	}
	if (meeple.getDetail() == MeepleDetail::HOLE){
		column += 2;
	}


	//std::cout << "size x|y: " << sizeX << "|" << sizeY << endl;
	unsigned int rectWidth = sizeX / 8;				//8 meeples in the sprite
	unsigned int rectHeight = sizeY / 2;			// two rows in the sprite
	unsigned int rectLeft = column * rectWidth;
	unsigned int rectTop = row * rectHeight;

	//cout << "tex row|column: " << row << "|" << column << endl;

	//std::cout << "rectWidth|rectHeight: " << rectWidth << "|" << rectHeight << endl;
	//std::cout << "rectLeft|rectTop: " << rectLeft << "|" << rectTop << endl << endl;
	return sf::IntRect (rectLeft, rectTop, rectWidth, rectHeight);
}

sf::IntRect Game::getGlowTextRectForPos(const Meeple& meeple){
	unsigned int row = 0;
	unsigned int column = 0;
	if (meeple.getShape() == MeepleShape::ROUND){
		row = 1;

	}
	if (meeple.getSize() == MeepleSize::SMALL){
		column = 1;
	}

	//unsigned int pos = 0;

	unsigned int sizeX = glowSprite.getSize().x;
	unsigned int sizeY = glowSprite.getSize().y;
	//std::cout << "size x|y: " << sizeX << "|" << sizeY << endl;
	unsigned int rectWidth = sizeX / 2;				//2 meeple columns in the sprite
	unsigned int rectHeight = sizeY / 2;			//2 meeple rows in the sprite
	
	unsigned int rectLeft = column * rectWidth;
	unsigned int rectTop = row * rectHeight;
	//std::cout << "rectWidth|rectHeight: " << rectWidth << "|" << rectHeight << endl;
	//std::cout << "rectLeft|rectTop: " << rectLeft << "|" << rectTop << endl << endl;
	cout << "glow row|column: " << row << "|" << column << endl;
	return sf::IntRect(rectLeft, rectTop, rectWidth, rectHeight);
}

void Game::loadTextures(){
	if (!meepleSprite.loadFromFile(WORKING_DIR + "pencilStyle.png")){
		std::cout << "couldn't load the texture: meepleSprites" << endl;
		// error...
	}
	if (!glowSprite.loadFromFile(WORKING_DIR + "glow.png")){
		std::cout << "couldn't load the texture: glow" << endl;
		// error...
	}
	if (!boardTexture.loadFromFile(WORKING_DIR + "board.png")){ 
		/*error */ 
	}
}

void Game::pollEvents(){
	sf::Event event;
	while (window->pollEvent(event)){
		sf::Vector2i mousepos = sf::Mouse::getPosition(*window);
		sf::Vector2f converted = window->mapPixelToCoords(mousepos);
		switch (event.type){
			case sf::Event::Resized:
				handleResizeWindowEvent(window);
				break;
			case sf::Event::Closed:
				window->close();
				break;
			case sf::Event::MouseButtonPressed:
				for (vector<RMeeple*>::iterator it = meeples.begin(); it != meeples.end(); ++it){
					if ((*it)->containsMousePos(converted)){
						(*it)->setIsDragged(true);
						
						mousePosRelativeToMeepleBoundary.x = converted.x - (*it)->getGlobalBounds().left;
						mousePosRelativeToMeepleBoundary.y = converted.y - (*it)->getGlobalBounds().top;

						break;
					}
					//else{
						//(*it)->setIsDragged(false);
					//}

				}
				break;
			case sf::Event::MouseButtonReleased:
				switch (event.mouseButton.button){
				case sf::Mouse::Left:
					for (vector<RMeeple*>::iterator it = meeples.begin(); it != meeples.end(); ++it){
						//if ((*it)->containsMousePos(converted)){
							(*it)->setIsDragged(false);
						//}
						//else{
							//(*it)->setIsDragged(true);
						//}
					}
					break;

				default:
					break;
				}
				break;
			case sf::Event::MouseMoved:
				for (vector<RMeeple*>::iterator it = meeples.begin(); it != meeples.end(); ++it){
					(*it)->shouldGlow(converted);
					if ((*it)->getIsDragged()){
						(*it)->updatePosition(sf::Vector2f(converted.x - mousePosRelativeToMeepleBoundary.x, converted.y - mousePosRelativeToMeepleBoundary.y));
					}
				}
				break;
			default:
				break;
		}

	}

	
}