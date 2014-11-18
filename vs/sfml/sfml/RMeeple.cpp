#include "RMeeple.h"

#include "Meeple.h"

//#include <iostream>
//using namespace std;

const float MEEPLE_HEIGHT = 170;
const float MEEPLE_ANCHOR_TOP = 140;

const float MEEPLE_WIDTH = 80;
const float MEEPLE_ANCHOR_LEFT = 40;

//logicalMeeple has to be deleted by the bag
RMeeple::RMeeple(const Meeple& logicalMeeple, sf::Texture& shapeTex, sf::Texture& glowShapeTex, sf::Vector2f& initPos)
	: logicalMeeple(&logicalMeeple), glowColor(nullptr)
{
	sf::Vector2f size(MEEPLE_WIDTH, MEEPLE_HEIGHT);

	shape.setSize(size);
	shapeTex.setSmooth(true);
	shape.setTexture(&shapeTex);

	glowShape.setSize(size);
	glowShapeTex.setSmooth(true);
	glowShape.setTexture(&glowShapeTex);

	setPosition(initPos);
	initTextRects(shapeTex,glowShapeTex);

}

RMeeple::~RMeeple(){ 
	delete glowColor;
}


void RMeeple::draw(sf::RenderWindow& window) const{
	window.draw(shape);
	if (glowColor != nullptr){
		window.draw(glowShape);
	}
}

void RMeeple::setPosition(const sf::Vector2f& newPosition){
	this->shape.setPosition(newPosition.x,newPosition.y);
	this->glowShape.setPosition(newPosition.x, newPosition.y);
	//cout << "new Position: " << newPosition.x << "|" << newPosition.y << endl;
}

const sf::Vector2f& RMeeple::getPosition()const{
	return this->shape.getPosition();
}

//sf::FloatRect RMeeple::getGlobalBounds() const{
//	return shape.getGlobalBounds();
//}

void RMeeple::setGlow(const sf::Color* color){
	delete glowColor;
	if (color == nullptr){
		glowColor = nullptr;
		return;
	}
	glowColor = new sf::Color(*color);
	glowShape.setFillColor(*glowColor);
}

bool RMeeple::isGlowing() const{
	return glowColor == nullptr;
}

bool RMeeple::containsPosition(sf::Vector2f& position) const{
	return shape.getGlobalBounds().contains(position);
}

sf::Vector2f RMeeple::getMousePosRelativeToMeepleBoundary(sf::Vector2f& mousePosition) const{
	return sf::Vector2f (mousePosition.x - shape.getGlobalBounds().left, mousePosition.y - shape.getGlobalBounds().top);
}


bool RMeeple::representsPassedMeeple(const Meeple* meeple) const{
	return logicalMeeple == meeple;
}


const Meeple* RMeeple::getLogicalMeeple() const{
	return this->logicalMeeple;
}


// quatsch weil ich den meeple in der hand habe und alle
// rfields fragen muss nicht umgekehrt
sf::Vector2f RMeeple::getAnchorPoint() const{
	sf::Vector2f anchorpoint;
	anchorpoint.x = shape.getGlobalBounds().left - MEEPLE_ANCHOR_LEFT;
	anchorpoint.y = shape.getGlobalBounds().top - MEEPLE_ANCHOR_TOP;
	return anchorpoint;
}


// ----------------------------------------------------------------------------
// private methods
// ----------------------------------------------------------------------------
void RMeeple::initTextRects(sf::Texture& meepleSprites, sf::Texture& meepleGlowSprites){

	unsigned int meepleSpriteRow = 0;
	unsigned int meepleSpriteColumn = 0;
	if (logicalMeeple->getColor() == MeepleColor::BLACK){
		meepleSpriteRow = 1;
	}
	if (logicalMeeple->getSize() == MeepleSize::SMALL){
		meepleSpriteColumn += 1;
	}
	if (logicalMeeple->getDetail() == MeepleDetail::HOLE){
		meepleSpriteColumn += 2;
	}
	if (logicalMeeple->getShape() == MeepleShape::ROUND){
		meepleSpriteColumn += 4;
	}

	unsigned int glowColumn = meepleSpriteColumn % 2 == 0 ? 0 : 1;
	unsigned int glowRow = meepleSpriteColumn < 4 ? 0 : 1;

	sf::IntRect texRect;

	texRect.width = meepleSprites.getSize().x / 8;				//8 meeples in the sprite
	texRect.height = meepleSprites.getSize().y / 2;			// two rows in the sprite
	texRect.left = meepleSpriteColumn * texRect.width;
	texRect.top = meepleSpriteRow * texRect.height;

	sf::IntRect glowRect;

	glowRect.width = meepleGlowSprites.getSize().x / 2;		//2 meeples in the sprite
	glowRect.height = meepleGlowSprites.getSize().y / 2;
	glowRect.left = glowColumn * glowRect.width;
	glowRect.top = glowRow * glowRect.height;

	shape.setTextureRect(texRect);
	glowShape.setTextureRect(glowRect);
	//cout << "tex meepleSpriteRow|meepleSpriteColumn: " << meepleSpriteRow << "|" << meepleSpriteColumn << endl;
	//std::cout << "rectWidth|rectHeight: " << rectWidth << "|" << rectHeight << endl;
	//std::cout << "rectLeft|rectTop: " << rectLeft << "|" << rectTop << endl << endl;
}