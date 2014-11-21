#pragma once

#include <SFML/Graphics.hpp>

#include "Board.h"
#include "RMeeple.h"

enum Shape{
	OCCUPIED,
	NOT_OCCUPIED
};

class RField
{
	

private:

	sf::RectangleShape fieldShape;
	sf::RectangleShape occupiedShape;

	sf::RectangleShape* shapePtr;

	BoardPos posOnBoard; //mapps tointernal state of ai

	Shape shape;

public:
	RField(const BoardPos& posOnBoard, sf::Vector2f& initPos, sf::Texture& fieldShape, sf::Texture& occupiedShape);
	~RField();

	void draw(sf::RenderWindow& window) const;

	bool containsPosition(sf::Vector2f& position) const;

	void setShape(Shape shape);

	sf::FloatRect getGlobalBounds() const;

	BoardPos getBoardPos() const;

};

