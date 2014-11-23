#pragma once

#include <SFML/Graphics.hpp>

#include "Board.h"
#include "RMeeple.h"

struct FieldShape{
    enum Enum{
        OCCUPIED,
        NOT_OCCUPIED
    };
};


class RField{
private:

	sf::RectangleShape fieldShape;
	sf::RectangleShape occupiedShape;

	sf::RectangleShape* shapePtr;

	BoardPos posOnBoard; //mapps tointernal state of ai

    FieldShape::Enum shape;

public:
	RField(const BoardPos& posOnBoard, sf::Vector2f& initPos, sf::Texture& fieldShape, sf::Texture& occupiedShape);
    void setShape(FieldShape::Enum shape);
	
	void draw(sf::RenderWindow& window) const;

	bool containsPosition(sf::Vector2f& position) const;
    sf::FloatRect getGlobalBounds() const;
	BoardPos getBoardPos() const;
};

