#include "RMeeple.h"

const float MEEPLE_HIGHT = 170;
const float MEEPLE_WIDTH = 80;

//logicalMeeple has to be deleted by the bag
RMeeple::RMeeple(const Meeple& logicalMeeple, sf::Texture& shapeTex, sf::IntRect& shapeTexRect, sf::Texture& glowShapeTex, sf::IntRect& glowShapeTexRect, sf::Vector2f& initPos)
	: logicalMeeple(&logicalMeeple), isDragged(false), glow(false)
{
	sf::Vector2f size(MEEPLE_WIDTH, MEEPLE_HIGHT);

	shape.setSize(size);
	shape.setTexture(&shapeTex);
	shape.setTextureRect(shapeTexRect);

	glowShape.setSize(size);
	glowShape.setTexture(&glowShapeTex);
	glowShape.setTextureRect(glowShapeTexRect);

	updatePosition(initPos);
}

RMeeple::~RMeeple(){ }


void RMeeple::updatePosition(const sf::Vector2f& newPosition){
	shape.setPosition(newPosition.x,newPosition.y);
	glowShape.setPosition(newPosition.x, newPosition.y);
}


void RMeeple::draw(sf::RenderWindow& window){
	window.draw(shape);
	if (glow){
		window.draw(glowShape);
	}
	
}

void RMeeple::shouldGlow(sf::Vector2f& converted){
	if (shape.getGlobalBounds().contains(converted)){
		glow = true;
	}
	else{
		glow = false;
	}
}

bool RMeeple::containsMousePos(sf::Vector2f& converted){
	return shape.getGlobalBounds().contains(converted);

		 //true;
		//isDragged = true;
		////return 
		//sf::Vector2f vect;
		//vect.x = converted.x - shape.getGlobalBounds().left;
		//vect.y = converted.y - shape.getGlobalBounds().top;
		//return vect;
	//}
	//else{
		//isDragged = false;
	//}
}

sf::FloatRect RMeeple::getGlobalBounds(){
	return shape.getGlobalBounds();
}

bool RMeeple::getIsDragged(){
	return this->isDragged;
}

void RMeeple::setIsDragged(bool isDragged){
	this->isDragged = isDragged;
}

bool RMeeple::getGlow(){
	return this->glow;
}

void RMeeple::setGlow(bool glow){
	this->glow = glow;
}

sf::Vector2f* RMeeple::getMoveToPosition(){
	return &this->moveToPosition;
}

void RMeeple::setMoveToPosition(sf::Vector2f& moveToPosition){
	this->moveToPosition.x = moveToPosition.x;
	this->moveToPosition.y = moveToPosition.y;
}






