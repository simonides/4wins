#include "ColorAnimation.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <assert.h>

#define M_PI       3.14159265358979323846f


sf::Color ColorAnimation::getColorByIndex(uint8_t idx) const{
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

ColorAnimation::ColorAnimation(float animationSpeed, uint8_t colorPosition){
    init(animationSpeed, colorPosition);
}

void ColorAnimation::init(float animationSpeed, uint8_t colorPosition){
    this->animationSpeed = animationSpeed;
    assert(colorPosition < 100);    //0..99
    nextColor = static_cast<uint8_t>(colorPosition / (100.f / 6.f));    //0..5
    assert(nextColor >= 0 && nextColor <= 5);
    progress = (100.f / 6.f) / (100.f - (100 / 6 * nextColor));
    assert(progress >= 0 && progress <= 1);
}

void ColorAnimation::animate(float elapsedTime){
    progress += animationSpeed * elapsedTime;

    uint8_t overflow = static_cast<uint8_t>(progress);
    progress -= overflow;
    nextColor = (nextColor + overflow) % 6;
}

void ColorAnimation::setSpeed(float animationSpeed){
    this->animationSpeed = animationSpeed;
}

sf::Color ColorAnimation::getColor(sf::Uint8 alpha) const{
        assert(progress <= 1);
        sf::Color f = getColorByIndex(nextColor - 1 >= 0 ? nextColor - 1 : 5); //from
        sf::Color t = getColorByIndex(nextColor);                              //to
        float from = progress;
        float to = 1.f - from;
        return sf::Color(static_cast<sf::Uint8>(f.r * to + t.r * from), static_cast<sf::Uint8>(f.g * to + t.g * from), static_cast<sf::Uint8>(f.b * to + t.b * from), alpha);

}
