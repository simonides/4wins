#include "RField.h"


const float FIELD_HEIGHT = 130.f;
const float FIELD_WIDTH = 130.f;

RField::RField(const BoardPos& posOnBoard, sf::Vector2f& initPos, sf::Texture& fieldShapeTex, sf::Texture& occupiedShapeTex)
: posOnBoard(posOnBoard), shape(FieldShape::NOT_OCCUPIED)
{
	fieldShape.setOrigin(sf::Vector2f(FIELD_WIDTH / 2, FIELD_HEIGHT / 2));
	fieldShape.setSize(sf::Vector2f(FIELD_WIDTH, FIELD_HEIGHT));
	fieldShape.setPosition(initPos);
	fieldShape.setTexture(&fieldShapeTex);
	//fieldShape.setFillColor(sf::Color::Yellow);
	//fieldShape.setFillColor(sf::Color(235,126,49,255));
	fieldShape.setFillColor(sf::Color(239, 229, 14, 255));


	sf::IntRect textRec(0, 0, fieldShapeTex.getSize().x, fieldShapeTex.getSize().y);
	fieldShape.setTextureRect(textRec);

	occupiedShape.setOrigin(sf::Vector2f(FIELD_WIDTH / 2, FIELD_HEIGHT / 2));
	occupiedShape.setSize(sf::Vector2f(FIELD_WIDTH, FIELD_HEIGHT));
	occupiedShape.setPosition(initPos);
	occupiedShape.setTexture(&occupiedShapeTex);
	occupiedShape.setTextureRect(textRec);
	occupiedShape.setFillColor(sf::Color::Magenta);

	shapePtr = &fieldShape;
}




void RField::draw(sf::RenderWindow& window) const{
	window.draw(*shapePtr);
}


bool RField::containsPosition(sf::Vector2f& position) const{
	return shapePtr->getGlobalBounds().contains(position);
}


void RField::setShape(FieldShape::Enum shape){
	this->shape = shape;
    shapePtr = (shape == FieldShape::OCCUPIED) ? &occupiedShape : &fieldShape;
}

sf::FloatRect RField::getGlobalBounds() const{
	return shapePtr->getGlobalBounds();
}

BoardPos RField::getBoardPos() const{
	return posOnBoard;
}
