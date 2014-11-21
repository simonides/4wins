#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <stdint.h>

namespace sf{
    class RenderWindow;
}


struct Particle{
    sf::RectangleShape shape;   
    sf::Vector2f direction;

    uint8_t nextColor;
    float colorProgress;   //0..255
    float colorSpeed;

    float alpha;
    float fadeoutSpeed;

    float rotationSpeed;
};




class ParticleSystem
{
private:
    sf::Vector2u gravity;

    sf::Texture particleSprites;
    sf::Vector2u spriteCount;   //Number of sprites in the texture
    sf::IntRect textureCoords;  //coords of the sprite at 0|0


    std::vector<Particle*> particles;


    void createNewParticle();
    sf::Color getColor(uint8_t idx);
    sf::Color getInterpolatedColor(uint8_t nextColor, float progress, sf::Uint8 alpha = 255);

    float randomise(float min, float max);
public:
    ParticleSystem();

    void update(float elapsedTime);
    void draw(sf::RenderWindow& window) const;


    ~ParticleSystem();
};

