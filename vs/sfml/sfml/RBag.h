#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class RMeeple;
class Meeple;

class RBag{
private:
	std::vector<RMeeple*> usedRMeeples;
	std::vector<RMeeple*> rMeeples;

public:
	~RBag();
	void draw(sf::RenderWindow& window);

	RMeeple* getRMeepleAtPosition(sf::Vector2f& position);  

	RMeeple* getRmeepleFromUsed(const Meeple* meeple) const;
    RMeeple* getRmeepleFromUnused(const Meeple* meeple);

	void changeRMeepleToUsed(const RMeeple& rMeeple);

	void addRMeeple(RMeeple* r_meeple);

	void reset();
};

