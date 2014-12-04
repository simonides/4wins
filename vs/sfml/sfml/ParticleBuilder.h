#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <stdint.h>

#include "ColorAnimation.h"
#include "helper.h"

struct Particle{
    //Position:
        sf::RectangleShape shape;
    //Path:
        sf::Vector2f direction;
        sf::Vector2f gravity;              
    //Color:
        bool staticColor;       //If true, the other color-fields are ignored, and the color will never be changed
        ColorAnimation colorAnimation;
    //Alpha:
        float alpha;
        float fadeoutSpeed;
    //Rotation:
        float rotationSpeed;
};


class ParticleBuilder{
private:
    //Position:
        sf::Vector2f position;         //Target position of the particle
        Interval positionOffset;       //Positionoffset   
    //Size:
        Interval diameter;             //Particle size
    //Path:
        Interval speed;                //Initial particle speed
        Interval angle;                //Initial particle direction (0°..360°)
        float gravitySpeed;            //gravity speed
        float gravity;                 //Gravity direction (0°..360°)
    //Rotation:
        Interval rotationOffset;       //Offset of the rotation-origin to the particle center (1 = diameter)
        Interval rotationSpeed;        //Rotationspeed (only positive)
    //Color:
        bool staticColor;               //true: only uses the "color"-member; false: iterpolates the color in all rainbow-colors by using the field "colorSpeed"
        sf::Color color;            
        Interval colorSpeed;
    //Alpha:
        Interval fadeoutSpeed;
    //Sprites:
        sf::Vector2u spritesX;               //the sprites of the texture, that can be used (0...number of sprites in the texture)
        sf::Vector2u spritesY;
public:
    ParticleBuilder(sf::Vector2f position, Interval diameter);
   
    ParticleBuilder* setPosition(sf::Vector2f position, Interval positionOffset={0,0});
    ParticleBuilder* setDiameter(Interval diameter);

    ParticleBuilder* setPath(Interval speed = { 40, 400 }, Interval angle = { 0, 360 });
    ParticleBuilder* setGravity(float gravitySpeed = 300, float gravity = 90);

    ParticleBuilder* setRotation(Interval rotationSpeed = { 0, 600 }, Interval rotationOffset = { 0.5, 1.5 });

    ParticleBuilder* setStaticColor(sf::Color color = sf::Color::White);
    ParticleBuilder* setDynamicColor(Interval colorSpeed = { 2, 8 });
    ParticleBuilder* setFadeoutSpeed(Interval fadeoutSpeed = { 50, 100 });
    ParticleBuilder* setSprites(sf::Vector2u spritesX = { 0, 0 }, sf::Vector2u spritesY = { 0, 0 });

    Particle* createParticle(sf::Texture& particleSprites, sf::Vector2u spriteCount, sf::IntRect textureCoords) const;     //Creates a new particle. Needs to be deleted()
    void initialiseParticle(Particle* memory, sf::Texture& particleSprites, sf::Vector2u spriteCount, sf::IntRect textureCoords) const;     
};