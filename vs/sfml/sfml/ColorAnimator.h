#pragma once
#include <stdint.h>
#include <SFML/System/Utf.hpp>

namespace sf{
    class Color;
}

class ColorAnimator{
public:
    static sf::Color getColor(uint8_t idx);
    static sf::Color getInterpolatedColor(uint8_t nextColor, float progress, sf::Uint8 alpha = 255);
};

