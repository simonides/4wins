#include "ColorAnimator.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <assert.h>



sf::Color ColorAnimator::getColor(uint8_t idx){
    switch (idx){
        case 0: return sf::Color::Red;
        case 1: return sf::Color::Yellow;
        case 2: return sf::Color::Green;
        case 3: return sf::Color::Cyan;
        case 4: return sf::Color::Blue;
        case 5: return sf::Color::Magenta;
        default: assert(false); return sf::Color::White;
    }
}

sf::Color ColorAnimator::getInterpolatedColor(uint8_t nextColor, float progress, sf::Uint8 alpha){
    assert(progress <= 255);
    sf::Color f = getColor(nextColor - 1 >= 0 ? nextColor - 1 : 5);
    sf::Color t = getColor(nextColor);
    float neg = 255.f - progress;
    return sf::Color(static_cast<sf::Uint8>(f.r / 255 * neg + t.r / 255 * progress), static_cast<sf::Uint8>(f.g / 255 * neg + t.g / 255 * progress), static_cast<sf::Uint8>(f.b / 255 * neg + t.b / 255 * progress), alpha);
}
