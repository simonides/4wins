#include "ParticleSystem.h"

#include <SFML/System.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <assert.h>
#include <stdint.h>
#include <cstdlib>

#include "config.h"
#include "ColorAnimator.h"

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


void ParticleSystem::newParticleCloud(unsigned int particleCount, ParticleBuilder& builder){
    for (unsigned int i = 0; i < particleCount; i++){
        particles.push_back(builder.createParticle(particleSprites, spriteCount, textureCoords));
    }
}


void ParticleSystem::update(float elapsedTime){
    assert(elapsedTime > 0);
    for (std::vector<Particle*>::const_iterator it = particles.begin(); it != particles.end();){
        //Path:
            (*it)->shape.move((*it)->direction.x * elapsedTime, (*it)->direction.y * elapsedTime);            
            (*it)->shape.move((*it)->gravity.x * elapsedTime, (*it)->gravity.y * elapsedTime);
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
            (*it)->shape.setFillColor(ColorAnimator::getInterpolatedColor((*it)->nextColor, (*it)->colorProgress, static_cast<sf::Uint8>((*it)->alpha)));
        //Delete if invisible:
            sf::Vector2f pos = (*it)->shape.getPosition();
            //TODO: simon fragen, wass ich hier für werte brauche
            if( (*it)->alpha <= 0 || 
                pos.x < -PARTICLE_DELETE_HORIZON || pos.x > WINDOW_WIDTH_TO_CALCULATE  + PARTICLE_DELETE_HORIZON ||
                pos.y < -PARTICLE_DELETE_HORIZON || pos.y > WINDOW_HEIGHT_TO_CALCULATE + PARTICLE_DELETE_HORIZON ){
                delete *it;
                it = particles.erase(it);
            }else{
                ++it;
            }
    }
}

void ParticleSystem::draw(sf::RenderWindow& window) const{
    for (std::vector<Particle*>::const_iterator it = particles.begin(); it != particles.end(); ++it){
        window.draw((*it)->shape);
    }
}
