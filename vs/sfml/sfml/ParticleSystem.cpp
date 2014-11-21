#include "ParticleSystem.h"

#include <SFML/System.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <assert.h>
#include <stdint.h>
#include <cstdlib>
#define M_PI       3.14159265358979323846f

#include "config.h"

#define PARTICLE_DELETE_HORIZON 50          //If a particle is outside of the window with a distance of PARTICLE_DELETE_HORIZON units, it will be deleted


ParticleSystem::ParticleSystem(){
    if (!particleSprites.loadFromFile(WORKING_DIR + "stars.png")){
        std::cerr << "couldn't load the texture: meepleSprites" << std::endl;
        assert(false);
    }
    spriteCount = sf::Vector2u(2, 2);

    textureCoords.width = particleSprites.getSize().x / spriteCount.x;				
    textureCoords.height = particleSprites.getSize().y / spriteCount.y;
    textureCoords.left = 0;
    textureCoords.top = 0;

}


ParticleSystem::~ParticleSystem(){
    for (std::vector<Particle*>::const_iterator it = particles.begin(); it != particles.end();){
        delete *it;
    }
}

void ParticleSystem::createNewParticle(){ 
   
    Particle* particle = new Particle();
    //Basics: 
        particle->shape.setTexture(&particleSprites);
        sf::IntRect coords = textureCoords;
        coords.left = coords.width * (rand() % spriteCount.x);
        coords.top = coords.height * (rand() % spriteCount.y);
        particle->shape.setTextureRect(coords);
         
    
    //Path:
        float speed = randomise(40, 400);
        float angle = randomise(0, 2.f*M_PI);
        particle->direction.x = cos(angle) * speed;
        particle->direction.y = sin(angle) * speed;    
    
        if (angle > 90 && angle < 270){
            particle->direction.x *= -1;
        }
        if (angle > 180){
            particle->direction.y *= -1;
        }

    //Size:
        float diameter = randomise(10, 60);
        particle->shape.setSize(sf::Vector2f(diameter, diameter));
    //Rotation:
        float offset = randomise(0.5, 1.5);
        particle->shape.setOrigin(diameter / 2.f * offset, diameter / 2.f * offset);    //origin = nearly center   
        float rotationPreference = randomise(50, 200);
        if (particle->direction.x < 0){
            rotationPreference *= -1;
        }
        particle->rotationSpeed = randomise(-300 + rotationPreference, 300 + rotationPreference);
    //Color:
        particle->nextColor = rand() % 6;
        particle->colorProgress = randomise(0, 255);
        particle->shape.setFillColor(getInterpolatedColor(particle->nextColor, particle->colorProgress));
        particle->colorSpeed = randomise(500, 2000);
    //Transparency:
        particle->alpha = 255.f;
        particle->fadeoutSpeed = randomise(50, 100);

        particle->shape.setPosition(sf::Vector2f(randomise(390, 410), randomise(390, 410)));
        particles.push_back(particle);
}

sf::Color ParticleSystem::getColor(uint8_t idx){
    switch(idx){
        case 0: return sf::Color::Red;
        case 1: return sf::Color::Yellow;
        case 2: return sf::Color::Green;
        case 3: return sf::Color::Cyan;
        case 4: return sf::Color::Blue;
        case 5: return sf::Color::Magenta;
        default: assert(false); return sf::Color::White;
    }
}

sf::Color ParticleSystem::getInterpolatedColor(uint8_t nextColor, float progress, sf::Uint8 alpha){
    sf::Color f = getColor(nextColor - 1 >= 0 ? nextColor - 1 : 5);
    sf::Color t = getColor(nextColor);
    float neg = 255.f - progress;
    return sf::Color(static_cast<sf::Uint8>(f.r / 255 * neg + t.r / 255 * progress), static_cast<sf::Uint8>(f.g / 255 * neg + t.g / 255 * progress), static_cast<sf::Uint8>(f.b / 255 * neg + t.b / 255 * progress), alpha);
}

float ParticleSystem::randomise(float min, float max){
    assert(max > min);
    float random = ((float)rand()) / (float)RAND_MAX;
    return  min + random * (max - min);
}

void ParticleSystem::update(float elapsedTime){
    assert(elapsedTime > 0);
    for (std::vector<Particle*>::const_iterator it = particles.begin(); it != particles.end();){
        //Path:
            (*it)->shape.move((*it)->direction.x * elapsedTime, (*it)->direction.y * elapsedTime);
            (*it)->direction.y += 200 * elapsedTime;        //gravity
        //Rotation:
            (*it)->shape.rotate((*it)->rotationSpeed * elapsedTime);
        //Color:
            (*it)->colorProgress += (*it)->colorSpeed *elapsedTime;
            while ((*it)->colorProgress > 255){
                (*it)->colorProgress -= 255;
                (*it)->nextColor = ((*it)->nextColor+1)%6;                
            }
        //Transparency:
            (*it)->alpha -= (*it)->fadeoutSpeed *elapsedTime;
            if ((*it)->alpha < 0){ (*it)->alpha = 0; }
            (*it)->shape.setFillColor(getInterpolatedColor((*it)->nextColor, (*it)->colorProgress, static_cast<sf::Uint8>((*it)->alpha)));

        //Delete if invisible:
            sf::Vector2f pos = (*it)->shape.getPosition();
            //TODO: simon fragen, wass ich hier für werte brauche
            if( (*it)->alpha <= 0 || 
                pos.x < -PARTICLE_DELETE_HORIZON || pos.x > WINDOW_WIDTH  + PARTICLE_DELETE_HORIZON ||
                pos.y < -PARTICLE_DELETE_HORIZON || pos.y > WINDOW_HEIGHT + PARTICLE_DELETE_HORIZON ){
                delete *it;
                it = particles.erase(it);
            }else{
                ++it;
            }
    }

    
    if (rand() % 100 < 3){
        int newParticles = randomise(0, rand()%200);
        for (int i = 0; i < newParticles; i++){
            createNewParticle();
        }
    }
}

void ParticleSystem::draw(sf::RenderWindow& window) const{
    for (std::vector<Particle*>::const_iterator it = particles.begin(); it != particles.end(); ++it){
        window.draw((*it)->shape);
    }
}