#pragma once

#include "RField.h"
#include "Board.h"

class RBoard
{
private:
	RField* fields[4][4];
	sf::RectangleShape boardShape;
	BoardPos hoveredField;

	void RBoard::initFields(sf::Texture& fieldTexture, sf::Texture& fieldTextureOccupied);
	BoardPos RBoard::getBoardPosforCounter(uint8_t counter);
public:
	RBoard(sf::Texture& boardTexture,  sf::Texture& fieldTexture,  sf::Texture& fieldTextureOccupied);
	~RBoard();
	
	void draw(sf::RenderWindow& window) const; 

	//returns {42,42} if no field is hovered
	BoardPos getBoardPosForPosititon(sf::Vector2f& position) const;

	//if x or y in board pos are higher than 3 no field is hovered
	void setHoveredField(const BoardPos boardPos);

	sf::FloatRect getFieldGlobalBounds(const BoardPos boardPos) const;

};

