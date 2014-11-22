#include "RMeeple.h"
#include "Meeple.h"
#include "config.h"

const float MEEPLE_HEIGHT_LARGE = 200.f;
const float MEEPLE_WIDTH = MEEPLE_HEIGHT_LARGE * 0.5f;

const float MEEPLE_HEIGHT_SMALL = MEEPLE_HEIGHT_LARGE * 0.60546875f;

const int TEX_SMALL_MEEPLE_TOP = 98;
const int TEX_SMALL_MEEPLE_HEIGHT = 158;
const int TEX_MEEPLE_WIDTH = 128;
const int TEX_LARGE_MEEPLE_HEIGHT = 256;


const float MEEPLE_ORIGIN_LEFT = MEEPLE_WIDTH / 2.f;

const float MEEPLE_ORIGIN_TOP_LARGE = MEEPLE_HEIGHT_LARGE * (1.f - 0.25f);
const float MEEPLE_ORIGIN_TOP_SMALL = MEEPLE_HEIGHT_SMALL *(1.f - 0.38f);




//logicalMeeple has to be deleted by the bag
RMeeple::RMeeple(const Meeple& logicalMeeple, sf::Texture& shapeTex, sf::Texture& glowShapeTex)
	: logicalMeeple(&logicalMeeple), glowColor(nullptr)
{

	sf::Vector2f size;
	size.x = MEEPLE_WIDTH;
	size.y = (logicalMeeple.getSize() == MeepleSize::BIG) ? MEEPLE_HEIGHT_LARGE : MEEPLE_HEIGHT_SMALL;

	if (logicalMeeple.getSize() == MeepleSize::BIG)
	{
		shape.setOrigin(MEEPLE_ORIGIN_LEFT, MEEPLE_ORIGIN_TOP_LARGE);
		glowShape.setOrigin(MEEPLE_ORIGIN_LEFT, MEEPLE_ORIGIN_TOP_LARGE);
	} else
	{
		shape.setOrigin(MEEPLE_ORIGIN_LEFT, MEEPLE_ORIGIN_TOP_SMALL);
		glowShape.setOrigin(MEEPLE_ORIGIN_LEFT, MEEPLE_ORIGIN_TOP_SMALL);
	}
		
	shape.setSize(size);
	//shape.setFillColor(sf::Color::Red);
	shapeTex.setSmooth(true);
	shape.setTexture(&shapeTex);

	glowShape.setSize(size);
	//glowShape.setFillColor(sf::Color::Black);
	glowShapeTex.setSmooth(true);
	glowShape.setTexture(&glowShapeTex);
	initTextRects(shapeTex, glowShapeTex);
	setPosition(getInitPositionForMeeple(logicalMeeple));
	

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
	return sf::Vector2f(mousePosition.x - shape.getGlobalBounds().left - shape.getOrigin().x, mousePosition.y - shape.getGlobalBounds().top - shape.getOrigin().y);
}


bool RMeeple::representsPassedMeeple(const Meeple* meeple) const{
	return logicalMeeple == meeple;
}


const Meeple* RMeeple::getLogicalMeeple() const{
	return this->logicalMeeple;
}


float RMeeple::getYPos() const{
	return shape.getGlobalBounds().top + shape.getGlobalBounds().height;
}

sf::Vector2f RMeeple::getGlobalOrigin() const
{
	sf::Vector2f returnVal;
	returnVal.x = shape.getGlobalBounds().left + shape.getOrigin().x;
	returnVal.y = shape.getGlobalBounds().top + shape.getOrigin().y;
	return  returnVal;
}


// ----------------------------------------------------------------------------
// private methods
// ----------------------------------------------------------------------------
void RMeeple::initTextRects(sf::Texture& , sf::Texture& ){

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

	texRect.width = TEX_MEEPLE_WIDTH;			
	texRect.height = (logicalMeeple->getSize() == MeepleSize::BIG )? TEX_LARGE_MEEPLE_HEIGHT : TEX_SMALL_MEEPLE_HEIGHT;
	texRect.left = meepleSpriteColumn * texRect.width;
	texRect.top = meepleSpriteRow * TEX_LARGE_MEEPLE_HEIGHT;

	sf::IntRect glowRect;

	glowRect.width = texRect.width;
	glowRect.height = texRect.height;
	glowRect.left = glowColumn * glowRect.width;
	glowRect.top = glowRow * TEX_LARGE_MEEPLE_HEIGHT;


	if (logicalMeeple->getSize() == MeepleSize::SMALL){

		texRect.top +=  TEX_SMALL_MEEPLE_TOP;

		glowRect.top += TEX_SMALL_MEEPLE_TOP;

	}
	

	shape.setTextureRect(texRect);
	glowShape.setTextureRect(glowRect);
	
}
void RMeeple::reset()
{
	setPosition( getInitPositionForMeeple(*logicalMeeple));
}


sf::Vector2f RMeeple::getInitPositionForMeeple(const Meeple& meeple) const
{
	float xCoord = 100.f;
	float yCoord = 90.f;


	if (meeple.getShape() == MeepleShape::ROUND && meeple.getDetail() == MeepleDetail::HOLE)
	{
		xCoord += 250.f;
		yCoord += 55.f;
	}
	if (meeple.getShape() == MeepleShape::ROUND && meeple.getDetail() == MeepleDetail::NO_HOLE)
	{
		xCoord += 342.f;
		yCoord += 112.f;
	}
	if (meeple.getShape() == MeepleShape::SQUARE && meeple.getDetail() == MeepleDetail::HOLE)
	{
		xCoord += 434.f;
		yCoord += 177.f;
	}
	if (meeple.getShape() == MeepleShape::SQUARE && meeple.getDetail() == MeepleDetail::NO_HOLE)
	{
		xCoord += 527.f;
		yCoord += 233.f;
	}

	if (meeple.getSize() == MeepleSize::SMALL)
	{
		xCoord += -65.f;
		yCoord += 44.f;
	}

	if (meeple.getColor() == MeepleColor::WHITE)
	{
		xCoord = WINDOW_WIDTH_TO_CALCULATE / 2.f - xCoord;
	}
	else
	{
		xCoord += WINDOW_WIDTH_TO_CALCULATE / 2.f;
	}
	return sf::Vector2f(xCoord, yCoord);
}