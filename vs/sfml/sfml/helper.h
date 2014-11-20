#pragma once

//todo: is this how you write a forward declaration for namspace-members?
namespace sf{
	class RenderWindow;
}

void handleResizeWindowEvent(sf::RenderWindow* window);

int getMaximum(int* intArray, unsigned int length);         //Returns the max. value within an int-array
float getAverage(int* intArray, unsigned int length);       //Returns the averaage value within an int-array
