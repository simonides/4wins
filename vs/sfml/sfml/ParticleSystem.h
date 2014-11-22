#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <stdint.h>

#include "ParticleBuilder.h"





namespace sf{
    class RenderWindow;
}





class ParticleSystem{
private:
    sf::Texture& particleSprites;
    sf::Vector2u spriteCount;   //Number of sprites in the texture
    sf::IntRect textureCoords;  //coords of the sprite at 0|0
    
    /*std::vector<Particle*> particles;
    std::vector<Particle*> particlePool;*/

    Particle* particles;  
    unsigned int particleCount;

    ParticleSystem& operator = (const ParticleSystem&);
public:
    ParticleSystem(sf::Texture& particleSprites, sf::Vector2u spriteCount);

    void newParticleCloud(unsigned int particleCount, ParticleBuilder& builder);    //Tells the particleSystem to generate new particles accoring to the building plan

    void update(float elapsedTime);
    void draw(sf::RenderWindow& window) const;

    ~ParticleSystem();
};

