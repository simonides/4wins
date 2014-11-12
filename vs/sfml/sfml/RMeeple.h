#pragma once
#include <SFML/Graphics.hpp>


class Meeple;
class I_Player;

class RMeeple
{
private:

	bool isdragged;

	sf::Vector2f* mouseOffsetToGlobalBounds;
	sf::Vector2f* lastValidPosition;

	sf::RectangleShape* graficRepresentation;

public:
	
	RMeeple(
			const Meeple& logicalMeeple, 
			sf::RectangleShape* graficRepresentation
			);
	
	~RMeeple();

	bool GetIsDragged();
	void SetIsDragged(bool isDragged);


	 

};

