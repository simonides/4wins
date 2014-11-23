#pragma once
#include <stdint.h>
#include <SFML/System/Utf.hpp>

namespace sf{
    class Color; 
}


class ColorAnimation{
    uint8_t nextColor;  //Index for the next color (ColorAnimation.h), to which we interpolate
    float progress;     //progress to the next color (0..1)
    float animationSpeed;

    sf::Color getColorByIndex(uint8_t idx) const;
public:
    ColorAnimation(float animationSpeed = 0, uint8_t colorPosition = rand() % 100); //color-position: any number between 0..99 (defines the color in the color circle)
    void init(float animationSpeed = 0, uint8_t colorPosition = rand() % 100);
    void setSpeed(float animationSpeed = 0);
    void animate(float elapsedTime);                                                //Animates the color
    sf::Color getColor(sf::Uint8 alpha = 255) const;  
};

