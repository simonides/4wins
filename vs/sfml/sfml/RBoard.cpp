#include "RBoard.h"
#include "Board.h"
#include "RField.h"

#include <iostream>
#include <assert.h>
#include "config.h"

const unsigned int BOARD_X_OFFSET = static_cast<int>( WINDOW_WIDTH_TO_CALCULATE / 2.f);
const unsigned int BOARD_Y_OFFSET = 152;

const unsigned int BOARD_X_SPACEING = 269;
const unsigned int BOARD_Y_SPACEING = 80;

const float BOARD_HEIGHT = 670.f;
const float BOARD_WIDTH = BOARD_HEIGHT * 1.615f; // 1,615 = Ratio 16:9

const float BOARD_X_POS = WINDOW_WIDTH_TO_CALCULATE /2.f ;
const float BOARD_Y_POS = WINDOW_HEIGHT_TO_CALCULATE / 2.f + 77.f;


RBoard::RBoard(  sf::Texture& boardTexture,sf::Texture& fieldTexture,   sf::Texture& fieldTextureOccupied)
	: hoveredField({42,42})
{
	boardShape.setTexture(&boardTexture);
	boardShape.setSize(sf::Vector2f(BOARD_WIDTH, BOARD_HEIGHT));
	sf::FloatRect bounds =  boardShape.getGlobalBounds();
	boardShape.setOrigin((bounds.left + bounds.width)/2.f, (bounds.top + bounds.height)/2.f);
	boardShape.setPosition(sf::Vector2f(BOARD_X_POS, BOARD_Y_POS));

	initFields(fieldTexture,fieldTextureOccupied);
}


RBoard::~RBoard(){
    for (uint8_t x = 0; x < 4; ++x){
        for (uint8_t y = 0; y < 4; ++y){
            delete fields[x][y];
        }
    }    
}

void RBoard::draw(sf::RenderWindow& window) const{
	window.draw(boardShape);
	for (uint8_t x = 0; x < 4; ++x){
		for (uint8_t y = 0; y < 4; ++y){
			fields[x][y]->draw(window);
		}
	}
}

BoardPos RBoard::getBoardPosForPosititon(sf::Vector2f& position) const{
	for (uint8_t x = 0; x < 4; ++x){
		for (uint8_t y = 0; y < 4; ++y){
			if (fields[x][y]->containsPosition(position)){
				return{ x, y };
			}
		}
	}
	return{ 42, 42 };
}

void RBoard::setHoveredField(const BoardPos boardPos){
	if (hoveredField.x < 4 && hoveredField.y < 4){
		fields[hoveredField.x][hoveredField.y]->setShape(FieldShape::NOT_OCCUPIED);
	}
	hoveredField = boardPos;
	if (boardPos.x < 4 && boardPos.y < 4){
        fields[boardPos.x][boardPos.y]->setShape(FieldShape::OCCUPIED);
	}
}


sf::FloatRect RBoard::getFieldGlobalBounds(const BoardPos boardPos) const{
	return fields[boardPos.x][boardPos.y]->getGlobalBounds();
}

sf::Vector2f RBoard::getFieldCoords(const BoardPos boardPos) const{
    sf::FloatRect bounds = fields[boardPos.x][boardPos.y]->getGlobalBounds();
    return{ bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f };
}


void RBoard::initFields(sf::Texture& fieldTexture, sf::Texture& fieldTextureOccupied){
	uint8_t counter = 0;

	for (uint8_t y = 0; y < 7; ++y){
		//cout << "counter in loop: " << counter << endl;
		int times = (y + 1) > 3 ? 7 - y : y + 1;
		float calcX = BOARD_X_OFFSET - ((times - 1) / 2.f *BOARD_X_SPACEING);

		for (int x = 0; x < times; ++x){
			BoardPos boardPos = getBoardPosforCounter(counter);
			//std::cout << "boardpos: " << boardPos.toString() << std::endl;
			sf::Vector2f fieldPos(calcX, static_cast<float>(y * BOARD_Y_SPACEING + BOARD_Y_OFFSET));
			fields[boardPos.x][boardPos.y] = new RField(boardPos, fieldPos, fieldTexture, fieldTextureOccupied);
			calcX += BOARD_X_SPACEING;
			++counter;
		}
	}
}

BoardPos RBoard::getBoardPosforCounter(uint8_t counter){
    switch (counter){
        case 0:	    return{ 0, 0 };
        case 2:		return{ 1, 0 };
        case 5:		return{ 2, 0 };
        case 9:		return{ 3, 0 };
        case 1:		return{ 0, 1 };
        case 4:		return{ 1, 1 };
        case 8:		return{ 2, 1 };
        case 12:	return{ 3, 1 };
        case 3:		return{ 0, 2 };
        case 7:		return{ 1, 2 };
        case 11:	return{ 2, 2 };
        case 14:	return{ 3, 2 };
        case 6:		return{ 0, 3 };
        case 10:	return{ 1, 3 };
        case 13:	return{ 2, 3 };
        case 15:	return{ 3, 3 };
        default:	assert(false); return{ 42, 42 };
    }
}