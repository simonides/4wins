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
	//fieldShape.setFillColor(sf::Color::Yellow);
	//fieldShape.setFillColor(sf::Color(235,126,49,255));
	fieldShape.setFillColor(sf::Color(239, 229, 14, 255));


	sf::IntRect textRec(0, 0, fieldShapeTex.getSize().x, fieldShapeTex.getSize().y);
	fieldShape.setTextureRect(textRec);

	occupiedShape.setOrigin(sf::Vector2f(WIDTH / 2, HEIGHT / 2));
	occupiedShape.setSize(sf::Vector2f(WIDTH, HEIGHT));
	occupiedShape.setPosition(initPos);
	occupiedShape.setTexture(&occupiedShapeTex);
	occupiedShape.setTextureRect(textRec);
	occupiedShape.setFillColor(sf::Color::Magenta);

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
	shapePtr = (shape == OCCUPIED) ? &occupiedShape : &fieldShape;
}

sf::FloatRect RField::getGlobalBounds() const{
	return shapePtr->getGlobalBounds();
}

BoardPos RField::getBoardPos() const{
	return posOnBoard;
}
