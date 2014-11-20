#include "RBag.h"

#include "RMeeple.h"

#include <vector>
#include <assert.h>

	RBag::RBag()
	{
		
	}
	RBag::~RBag()
	{
		for (std::vector<RMeeple*>::iterator it = rMeeples.begin(); it != rMeeples.end(); ++it){
			delete *it;
		}
		for (std::vector<RMeeple*>::iterator it = usedRMeeples.begin(); it != usedRMeeples.end(); ++it){
			delete *it;
		}
	}
	void RBag::draw(sf::RenderWindow& window)
	{
		for (std::vector<RMeeple*>::iterator it = rMeeples.begin(); it != rMeeples.end(); ++it){
			(*it)->draw(window);
		}
		for (std::vector<RMeeple*>::iterator it = usedRMeeples.begin(); it != usedRMeeples.end(); ++it){
			(*it)->draw(window);
		}
	}

	//returns only UNused meeples 
	RMeeple* RBag::getRMeepleAtPosition(sf::Vector2f& position)
	{
		for (std::vector<RMeeple*>::const_iterator it = rMeeples.begin(); it != rMeeples.end(); ++it){
			if ((*it)->containsPosition(position)){
				return *it;
			}
		}
		return nullptr;
	}

	RMeeple* RBag::isPassedMeepleInUsed(const Meeple* meeple) const
	{
		assert(meeple != nullptr);
		for (std::vector<RMeeple*>::const_iterator it = usedRMeeples.begin(); it != usedRMeeples.end(); ++it){
			if ((*it)->representsPassedMeeple(meeple)){
				return *it;
			}
		}
		return nullptr;
	}

	RMeeple* RBag::isPassedMeepleInUnused(const Meeple* meeple) const
	{
		assert(meeple != nullptr);
		for (std::vector<RMeeple*>::const_iterator it = rMeeples.begin(); it != rMeeples.end(); ++it){
			if ((*it)->representsPassedMeeple(meeple)){
				return *it;
			}
		}
		return nullptr;
	}

	void RBag::changeRMeepleToUsed(const RMeeple& rMeeple)
	{
		//Todo unbedingt y sortieren!!!!!!!!!!!!!!!!!!!!!!

		//mylist.insert()
		//if (someIterator != someList.end()) {
		//someIterator++;
		//}
		//someList.insert(someIterator, someValue);

		for (std::vector<RMeeple*>::iterator it = rMeeples.begin(); it != rMeeples.end(); ++it){
			if (&rMeeple == *it){
				RMeeple* m = *it;
				rMeeples.erase(it);
				usedRMeeples.push_back(m);
				break;
			}
		}
	}

	void RBag::addRMeeple(RMeeple* r_meeple)
	{
		rMeeples.push_back(r_meeple);
	}

	void RBag::reset()
	{
		rMeeples.insert(rMeeples.end(), usedRMeeples.begin(), usedRMeeples.end());
		usedRMeeples.clear();
		assert(rMeeples.size() == 8 && usedRMeeples.size() == 0);
	}

