#include "ParticleBuilder.h"
#include <assert.h>
#include <cstdlib>
#include <iostream>


#include "ColorAnimator.h"
#define M_PI       3.14159265358979323846f



float Interval::get(){
    assert(max >= min);
    float random = ((float)rand()) / (float)RAND_MAX;
    return  min + random * (max - min);
}




ParticleBuilder::ParticleBuilder(sf::Vector2f position, Interval diameter, Interval speed, Interval angle, Interval colorSpeed, Interval fadeoutSpeed) :
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
}

void ParticleBuilder::setPosition(sf::Vector2f position){
    this->position = position;
}

void ParticleBuilder::setPositionOffset(Interval positionOffset){
    this->positionOffset = positionOffset;
}

void ParticleBuilder::setGravity(float gravitySpeed, float gravity){
    this->gravitySpeed = gravitySpeed;
    this->gravity = gravity;
}

void ParticleBuilder::setRotation(Interval rotationOffset, Interval rotationSpeed){
    assert(rotationSpeed.min >= 0);
    this->rotationOffset = rotationOffset;
    this->rotationSpeed = rotationSpeed;
}


Particle* ParticleBuilder::createParticle(sf::Texture& particleSprites, sf::Vector2u spriteCount, sf::IntRect textureCoords){
    Particle* particle = new Particle();
    //Basics: 
        particle->shape.setTexture(&particleSprites);
        sf::IntRect coords = textureCoords;
        coords.left = coords.width * (rand() % spriteCount.x);
        coords.top = coords.height * (rand() % spriteCount.y);
        particle->shape.setTextureRect(coords);
    //Path:
        float _speed = speed.get();
        float _angle = angle.get()*M_PI / 180.f;
        particle->direction.x = cos(_angle) * _speed;
        particle->direction.y = sin(_angle) * _speed;
    //Gravity:
        float angle_rad = gravity*M_PI / 180.f;
        particle->gravity.x = cos(angle_rad) * gravitySpeed;
        particle->gravity.y = sin(angle_rad) * gravitySpeed;
    //Size:
        float _diameter = diameter.get();
        particle->shape.setSize(sf::Vector2f(_diameter, _diameter));
    //Rotation:
        float _rotationOffset = rotationOffset.get();
        particle->shape.setOrigin(_diameter / 2.f * _rotationOffset, _diameter / 2.f * _rotationOffset);    //origin = nearly center   
                
        float diff = rotationSpeed.max - rotationSpeed.min;
        Interval _rotationSpeed = { -(diff / 2.f), diff / 2.f };
        Interval rotationPreference = { diff * 0.1, diff * 0.3 };
        float _rotationPreference = rotationPreference.get();
        if (particle->direction.x < 0){
            _rotationPreference *= -1;
        }
        _rotationSpeed.min += _rotationPreference;
        _rotationSpeed.max += _rotationPreference;
        
        particle->rotationSpeed = _rotationSpeed.get();
    //Color:
        particle->nextColor = rand() % 6;
        Interval colorProgress = { 0, 255 };
        particle->colorProgress = colorProgress.get();


        particle->shape.setFillColor(ColorAnimator::getInterpolatedColor(particle->nextColor, particle->colorProgress));
        particle->colorSpeed = colorSpeed.get();
    //Transparency:
        particle->alpha = 255.f;
        particle->fadeoutSpeed = fadeoutSpeed.get();

        sf::Vector2f _position = position;
        _position.x += positionOffset.get();
        _position.y += positionOffset.get();
        
        particle->shape.setPosition(_position);
    
        return particle;
}