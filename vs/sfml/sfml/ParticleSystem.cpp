#include "ParticleSystem.h"

#include <SFML/System.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <assert.h>
#include <stdint.h>
#include <cstdlib>

#include "config.h"
#include "ColorAnimation.h"

#define PARTICLE_DELETE_HORIZON 30          //If a particle is outside of the window with a distance of PARTICLE_DELETE_HORIZON units, it will be deleted


ParticleSystem::ParticleSystem(sf::Texture& particleSprites, sf::Vector2u spriteCount) : particleSprites(particleSprites), spriteCount(spriteCount){
    textureCoords.width = particleSprites.getSize().x / spriteCount.x;				
    textureCoords.height = particleSprites.getSize().y / spriteCount.y;
    textureCoords.left = 0;
    textureCoords.top = 0;

    particles = new Particle[MAX_PARTICLE_COUNT];       //Allocate everything - we don't wanna have any new's during the game
    particleCount = 0;
}

ParticleSystem::~ParticleSystem(){
    delete[] particles;
}


void ParticleSystem::newParticleCloud(unsigned int newParticleCount, ParticleBuilder& builder){
    for (unsigned int i = particleCount; i < particleCount + newParticleCount; ++i){
        if (i >= MAX_PARTICLE_COUNT){
            #ifndef NDEBUG
                std::cerr << "Particle Limit reached";
            #endif
                particleCount = MAX_PARTICLE_COUNT;
                return;
        }
        builder.initialiseParticle(particles+i, particleSprites, spriteCount, textureCoords);
    }
    particleCount += newParticleCount;
}


void ParticleSystem::update(float elapsedTime){
    assert(elapsedTime > 0);

    for (unsigned int i = 0; i < particleCount; ++i){
        //Path:
		//const sf::FloatRect& particlePosBefore = particles[i].shape.getGlobalBounds();
             particles[i].shape.move( particles[i].direction.x * elapsedTime,  particles[i].direction.y * elapsedTime);            
             particles[i].shape.move( particles[i].gravity.x * elapsedTime,  particles[i].gravity.y * elapsedTime);
		//const sf::FloatRect& particlePosAfter = particles[i].shape.getGlobalBounds();
		//particles[i].direction.x = (particlePosAfter.left - particlePosBefore.left) / elapsedTime;
		//particles[i].direction.y = (particlePosAfter.top - particlePosBefore.top) / elapsedTime;
        //Rotation:
             particles[i].shape.rotate( particles[i].rotationSpeed * elapsedTime);
        //Transparency:
             particles[i].alpha -= particles[i].fadeoutSpeed * elapsedTime;
             if (particles[i].alpha < 0){ particles[i].alpha = 0; }          
        //Color:
             if (!particles[i].staticColor){            //Do we have dynamic colors?
                 particles[i].colorAnimation.animate(elapsedTime);              
                 particles[i].shape.setFillColor(particles[i].colorAnimation.getColor(static_cast<sf::Uint8>(particles[i].alpha)));
             }else{
                 sf::Color color = particles[i].shape.getFillColor();
                 color.a = static_cast<sf::Uint8>(particles[i].alpha);
                 particles[i].shape.setFillColor(color);
             }             
        //Delete if invisible:
            sf::Vector2f pos =  particles[i].shape.getPosition();
            if( particles[i].alpha <= 0.1 || 
                pos.x < -PARTICLE_DELETE_HORIZON || pos.x > WINDOW_WIDTH_TO_CALCULATE  + PARTICLE_DELETE_HORIZON ||
                pos.y < -PARTICLE_DELETE_HORIZON || pos.y > WINDOW_HEIGHT_TO_CALCULATE + PARTICLE_DELETE_HORIZON ){

                --particleCount;
                if (i != particleCount){
                    particles[i] = particles[particleCount];
                }
            }
    }
}

void ParticleSystem::draw(sf::RenderWindow& window) const{
    for (unsigned int i = 0; i < particleCount; ++i){
        window.draw(particles[i].shape);
    }
}
