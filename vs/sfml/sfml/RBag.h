#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class RMeeple;
class Meeple;

class RBag
{
private:
	std::vector<RMeeple*> usedRMeeples;
	std::vector<RMeeple*> rMeeples;

public:
	RBag();
	~RBag();
	void draw(sf::RenderWindow& window);

	RMeeple* getRMeepleAtPosition(sf::Vector2f& position);
//Todo
	RMeeple* isPassedMeepleInUsed(const Meeple* meeple) const;
	RMeeple* isPassedMeepleInUnused(const Meeple* meeple) const;

	void changeRMeepleToUsed(const RMeeple& rMeeple);

	void addRMeeple(RMeeple* r_meeple);

	void reset();
};

