#include "RBag.h"

#include <vector>
#include <assert.h>

#include "RMeeple.h"


RBag::~RBag(){
    for (std::vector<RMeeple*>::iterator it = rMeeples.begin(); it != rMeeples.end(); ++it){
        delete *it;
    }
    for (std::vector<RMeeple*>::iterator it = usedRMeeples.begin(); it != usedRMeeples.end(); ++it){
        delete *it;
    }
}
void RBag::draw(sf::RenderWindow& window){
    for (std::vector<RMeeple*>::iterator it = rMeeples.begin(); it != rMeeples.end(); ++it){
        (*it)->draw(window);
    }
    for (std::vector<RMeeple*>::iterator it = usedRMeeples.begin(); it != usedRMeeples.end(); ++it){
        (*it)->draw(window);
    }
}

//returns only UNused meeples 
RMeeple* RBag::getRMeepleAtPosition(sf::Vector2f& position) const{
    for (std::vector<RMeeple*>::const_iterator it = rMeeples.begin(); it != rMeeples.end(); ++it){
        if ((*it)->containsPosition(position)){
            return *it;
        }
    }
    return nullptr;
}

RMeeple* RBag::getRmeepleFromUsed(const Meeple* meeple) const{
    assert(meeple != nullptr);
    for (std::vector<RMeeple*>::const_iterator it = usedRMeeples.begin(); it != usedRMeeples.end(); ++it){
        if ((*it)->representsPassedMeeple(meeple)){
            return *it;
        }
    }
    return nullptr;
}

RMeeple* RBag::getRmeepleFromUnused(const Meeple* meeple){
    assert(meeple != nullptr);
    for (std::vector<RMeeple*>::const_iterator it = rMeeples.begin(); it != rMeeples.end(); ++it){
        if ((*it)->representsPassedMeeple(meeple)){
            return *it;
        }
    }
    return nullptr;
}

void RBag::changeRMeepleToUsed(const RMeeple& rMeeple){
    for (std::vector<RMeeple*>::iterator it = rMeeples.begin(); it != rMeeples.end(); ++it){
        if (&rMeeple == *it){
            RMeeple* m = *it;
            rMeeples.erase(it);
            usedRMeeples.push_back(m);
            break;
        }
    }
}

void RBag::addRMeeple(RMeeple* r_meeple){
    rMeeples.push_back(r_meeple);
    sort(rMeeples.begin(), rMeeples.end(), [](RMeeple* a, RMeeple* b){return a->getYPos() > b->getYPos(); });
}

void RBag::reset(){
    rMeeples.insert(rMeeples.end(), usedRMeeples.begin(), usedRMeeples.end());
    usedRMeeples.clear();
    assert(rMeeples.size() == 8 && usedRMeeples.size() == 0);
    for (std::vector<RMeeple*>::iterator it = rMeeples.begin(); it != rMeeples.end(); ++it){
        (*it)->reset();
    }
	sort(rMeeples.begin(), rMeeples.end(), [](RMeeple* a, RMeeple* b){return a->getYPos() > b->getYPos(); });
}

