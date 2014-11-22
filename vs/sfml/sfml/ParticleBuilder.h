#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <stdint.h>


struct Particle{
    //Position:
        sf::RectangleShape shape;
    //Path:
        sf::Vector2f direction;
        sf::Vector2f gravity;              
    //Color:
        uint8_t nextColor;
        float colorProgress;   //0..255
        float colorSpeed;
    //Alpha:
        float alpha;
        float fadeoutSpeed;
    //Rotation:
        float rotationSpeed;
};


struct Interval{
    float min;
    float max;
    float get() const;
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
    Interval colorSpeed;
    //Alpha:
    Interval fadeoutSpeed;

public:
    ParticleBuilder(sf::Vector2f position, Interval diameter);
    //ParticleBuilder(sf::Vector2f position, Interval diameter, Interval speed = { 40, 400 }, Interval angle = { 0, 360 }, Interval colorSpeed = { 500, 2000 }, Interval fadeoutSpeed = { 50, 100 });
    ParticleBuilder* setPosition(sf::Vector2f position, Interval positionOffset={0,0});
    ParticleBuilder* setDiameter(Interval diameter);

    ParticleBuilder* setPath(Interval speed = { 40, 400 }, Interval angle = { 0, 360 });
    ParticleBuilder* setGravity(float gravitySpeed = 300, float gravity = 90);

    ParticleBuilder* setRotation(Interval rotationSpeed = { 0, 600 }, Interval rotationOffset = { 0.5, 1.5 });

    ParticleBuilder* setColorSpeed(Interval colorSpeed = { 500, 2000 });
    ParticleBuilder* setFadeoutSpeed(Interval fadeoutSpeed = { 50, 100 });
    
    Particle* createParticle(sf::Texture& particleSprites, sf::Vector2u spriteCount, sf::IntRect textureCoords) const;     //Creates a new particle. Needs to be deleted()
    void initialiseParticle(Particle* memory, sf::Texture& particleSprites, sf::Vector2u spriteCount, sf::IntRect textureCoords) const;     
};