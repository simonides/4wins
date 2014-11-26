#include "ParticleBuilder.h"
#include <assert.h>
#include <cstdlib>
#include <iostream>


#include "ColorAnimation.h"
#define M_PI       3.14159265358979323846f





/*ParticleBuilder::ParticleBuilder(sf::Vector2f position, Interval diameter, Interval speed, Interval angle, Interval colorSpeed, Interval fadeoutSpeed) :
        position(position),
        positionOffset({ 0, 0 }),
        diameter(diameter),
        speed(speed),
        angle(angle),
        gravitySpeed(0),
        gravity(270),
        rotationOffset({ 0, 0 }),
        rotationSpeed({ 0, 0 }),
        colorSpeed(colorSpeed),
        fadeoutSpeed(fadeoutSpeed){
}*/


ParticleBuilder::ParticleBuilder(sf::Vector2f position, Interval diameter){ 
    setPosition(position);
    setDiameter(diameter);
    setPath();
    setGravity();
    setRotation();
    setStaticColor();
    setFadeoutSpeed();
    setSprites();
}

ParticleBuilder* ParticleBuilder::setDiameter(Interval diameter){
    this->diameter = diameter;
    return this;
}

ParticleBuilder*  ParticleBuilder::setPosition(sf::Vector2f position, Interval positionOffset){
    this->position = position;
    this->positionOffset = positionOffset;
    return this;
}


ParticleBuilder* ParticleBuilder::setPath(Interval speed, Interval angle){
    this->speed = speed;
    this->angle = angle;
    return this;
}

ParticleBuilder* ParticleBuilder::setGravity(float gravitySpeed, float gravity){
    this->gravitySpeed = gravitySpeed;
    this->gravity = gravity;
    return this;
}

ParticleBuilder* ParticleBuilder::setRotation(Interval rotationSpeed, Interval rotationOffset){
    this->rotationSpeed = rotationSpeed;
    this->rotationOffset = rotationOffset;
    return this;
}

ParticleBuilder* ParticleBuilder::setStaticColor(sf::Color color){
    staticColor = true;
    this->color = color;
    return this;
}

ParticleBuilder* ParticleBuilder::setDynamicColor(Interval colorSpeed){
    staticColor = false;
    this->colorSpeed = colorSpeed;
    return this;
}

ParticleBuilder* ParticleBuilder::setFadeoutSpeed(Interval fadeoutSpeed){
    this->fadeoutSpeed = fadeoutSpeed;
    return this;
}

ParticleBuilder* ParticleBuilder::setSprites(sf::Vector2u spritesX, sf::Vector2u spritesY){
    this->spritesX = spritesX;
    this->spritesY = spritesY;
    return this;
}

void ParticleBuilder::initialiseParticle(Particle* memory, sf::Texture& particleSprites, sf::Vector2u spriteCount, sf::IntRect textureCoords) const{
    //Basics: 
        memory->shape.setTexture(&particleSprites);
        sf::IntRect coords = textureCoords;
        sf::Vector2u sprite;
        sprite.x = (spritesX.x == spritesX.y) ? spritesX.x : rand() % (spritesX.y - spritesX.x) + spritesX.x;
        sprite.y = (spritesY.x == spritesY.y) ? spritesY.x : rand() % (spritesY.y - spritesY.x) + spritesY.x;
        (void)spriteCount;  //avoid unused parameter warning
        assert(sprite.x < spriteCount.x && sprite.y < spriteCount.y);
        coords.left = coords.width * sprite.x;
        coords.top = coords.height * sprite.y;
        memory->shape.setTextureRect(coords);
    //Path:
        float _speed = speed.get();
        float _angle = angle.get()*M_PI / 180.f;
        memory->direction.x = cos(_angle) * _speed;
        memory->direction.y = sin(_angle) * _speed;
    //Gravity:
        float angle_rad = gravity*M_PI / 180.f;
        memory->gravity.x = cos(angle_rad) * gravitySpeed;
        memory->gravity.y = sin(angle_rad) * gravitySpeed;
    //Size:
        float _diameter = diameter.get();
        memory->shape.setSize(sf::Vector2f(_diameter, _diameter));
    //Rotation:
        float _rotationOffset = rotationOffset.get();
        memory->shape.setOrigin(_diameter / 2.f * _rotationOffset, _diameter / 2.f * _rotationOffset);    //origin = nearly center   

        float diff = rotationSpeed.max - rotationSpeed.min;
        Interval _rotationSpeed = { -(diff / 2.f), diff / 2.f };
        Interval rotationPreference = { diff * 0.1f, diff * 0.3f };
        float _rotationPreference = rotationPreference.get();
        if (memory->direction.x < 0){
            _rotationPreference *= -1;
        }
        _rotationSpeed.min += _rotationPreference;
        _rotationSpeed.max += _rotationPreference;

        memory->rotationSpeed = _rotationSpeed.get();
    //Transparency:
        memory->alpha = 255.f;
        memory->fadeoutSpeed = fadeoutSpeed.get();
    //Color:
        memory->staticColor = staticColor;
        if (memory->staticColor){
            memory->shape.setFillColor(color);
        }else{
            memory->colorAnimation.setSpeed(colorSpeed.get() );
            memory->shape.setFillColor(memory->colorAnimation.getColor(static_cast<sf::Uint8>(memory->alpha)));
        }
    //Position:
        sf::Vector2f _position = position;
        _position.x += positionOffset.get();
        _position.y += positionOffset.get();

        memory->shape.setPosition(_position);
}

Particle* ParticleBuilder::createParticle(sf::Texture& particleSprites, sf::Vector2u spriteCount, sf::IntRect textureCoords) const{
    Particle* particle = new Particle();
    initialiseParticle(particle, particleSprites, spriteCount, textureCoords);
    return particle;
}