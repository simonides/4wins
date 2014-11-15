#pragma once
#include <SFML/Graphics.hpp>


class Meeple;
class I_Player;

class RMeeple
{
private:

	const Meeple* logicalMeeple;

	sf::RectangleShape shape; //pos is in there
	sf::RectangleShape glowShape;
	
	sf::Vector2f moveToPosition;

	bool isDragged;
	bool glow;
	bool isLocked;

public:
	RMeeple(const Meeple& logicalMeeple, sf::Texture& shapeTex, sf::IntRect& shapeTexRect, sf::Texture& glowTexShape, sf::IntRect& glowTexShapeRect, sf::Vector2f& initPos);
	~RMeeple();
	
	void updatePosition(const sf::Vector2f& newPosition);
	void draw(sf::RenderWindow& window);

	bool getIsDragged();
	void setIsDragged(bool isDragged);

	bool getGlow();
	void setGlow(bool glow);

	sf::Vector2f* getMoveToPosition();
	void setMoveToPosition(sf::Vector2f& moveToPosition);

	void shouldGlow(sf::Vector2f& converted);

	bool containsMousePos(sf::Vector2f& converted);

	sf::FloatRect getGlobalBounds();
};

