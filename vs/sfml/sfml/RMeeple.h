#pragma once
#include <SFML/Graphics.hpp>


class Meeple;

class RMeeple
{
private:

	const Meeple* logicalMeeple;

	sf::RectangleShape shape; //pos is in there
	sf::RectangleShape glowShape;
	
	sf::Color* glowColor;
	
	void initTextRects(sf::Texture& meepleSprites, sf::Texture& meepleGlowSprites);

public:
	RMeeple(const Meeple& logicalMeeple, sf::Texture& shapeTex, sf::Texture& glowTexShape);
	~RMeeple();
	
	void draw(sf::RenderWindow& window) const;

	void setPosition(const sf::Vector2f& newPosition);
	const sf::Vector2f& getPosition() const;

	void setGlow(const sf::Color* color);

	bool isGlowing() const;

	bool containsPosition(sf::Vector2f& position) const;
	sf::Vector2f getMousePosRelativeToMeepleBoundary(sf::Vector2f& mousePosition) const;

	bool representsPassedMeeple(const Meeple* meeple) const;

	const Meeple* getLogicalMeeple() const;

	float RMeeple::getYPos() const;
		
	sf::Vector2f RMeeple::getGlobalOrigin() const;

	void reset();

	sf::Vector2f getInitPositionForMeeple(const Meeple& meeple) const;
};
