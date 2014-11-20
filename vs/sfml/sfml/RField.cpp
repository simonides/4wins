#include "RField.h"


const float HEIGHT = 130.f;
const float WIDTH = 130.f;

RField::RField(const BoardPos& posOnBoard, sf::Vector2f& initPos, sf::Texture& fieldShapeTex, sf::Texture& occupiedShapeTex)
	: posOnBoard(posOnBoard),shape(NOT_OCCUPIED)
{
	fieldShape.setOrigin(sf::Vector2f(WIDTH / 2, HEIGHT / 2));
	fieldShape.setSize(sf::Vector2f(WIDTH, HEIGHT));
	fieldShape.setPosition(initPos);
	fieldShape.setTexture(&fieldShapeTex);

	sf::IntRect textRec(0, 0, 342, 342);
	fieldShape.setTextureRect(textRec);

	occupiedShape.setOrigin(sf::Vector2f(WIDTH / 2, HEIGHT / 2));
	occupiedShape.setSize(sf::Vector2f(WIDTH, HEIGHT));
	occupiedShape.setPosition(initPos);
	occupiedShape.setTexture(&occupiedShapeTex);
	occupiedShape.setTextureRect(textRec);

	shapePtr = &fieldShape;


}


RField::~RField()
{
}


void RField::draw(sf::RenderWindow& window) const{
	window.draw(*shapePtr);
}


bool RField::containsPosition(sf::Vector2f& position) const{
	return shapePtr->getGlobalBounds().contains(position);
}


void RField::setShape(Shape shape){
	
	this->shape = shape;
	shapePtr = shape == Shape::OCCUPIED ? &occupiedShape : &fieldShape;
}

const sf::FloatRect RField::getGlobalBounds() const{
	//TODO eventuell schon die bereinigten(meeple einbeziehen) koords zurückgeben

	return shapePtr->getGlobalBounds();
}

BoardPos RField::getBoardPos() const{
	return posOnBoard;
}

//removes hover effect only if shape is flase too
//void RField::setHoverEffect(bool setEffect){
//	shapePtr = &occupiedShape;
//	if (!setEffect && shape == Shape::NOT_OCCUPIED){
//		shapePtr =  &fieldShape;
//	}
//}