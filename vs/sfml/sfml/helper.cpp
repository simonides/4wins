#include "helper.h"

#include <assert.h>
#include <SFML/Graphics.hpp>

void handleResizeWindowEvent(sf::RenderWindow* window){
	if (window->getSize().y == 0){ return; }
	float ratio = static_cast<float>(window->getSize().x) / static_cast<float>(window->getSize().y);
	if (!(ratio >= 1.75f && ratio <= 1.8)){
		window->setSize(sf::Vector2u(window->getSize().x, static_cast<unsigned int>(static_cast<float>(window->getSize().x) / 16.f * 9.f)));
	}
}

int getMaximum(int* intArray, unsigned int length){
    assert(length > 0);
    int max = intArray[0];
    for (unsigned int i = 1; i < length; ++i){
        if (intArray[i] > max){
            max = intArray[i];
        }
    }
    return max;
}

float getAverage(int* intArray, unsigned int length){
    assert(length > 0);
    long sum = 0;
    for (unsigned int i = 0; i < length; ++i){
        sum += intArray[i];
    }
    return sum / (float)length;
}
