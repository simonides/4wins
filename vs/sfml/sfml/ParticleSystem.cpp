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
    //TODO: don't load texture here
    if (!particleSprites.loadFromFile(WORKING_DIR + "stars.png")){
        std::cerr << "couldn't load the texture: meepleSprites" << std::endl;
        assert(false);
    }
    spriteCount = sf::Vector2u(2, 2);

    textureCoords.width = particleSprites.getSize().x / spriteCount.x;				
    textureCoords.height = particleSprites.getSize().y / spriteCount.y;
    textureCoords.left = 0;
    textureCoords.top = 0;

    particles = new Particle[MAX_PARTICLE_COUNT];       //Allocate everything - we don't wanna have any new's during the game
    particleCount = 0;
}

ParticleSystem::~ParticleSystem(){
    delete particles;
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
             particles[i].shape.move( particles[i].direction.x * elapsedTime,  particles[i].direction.y * elapsedTime);            
             particles[i].shape.move( particles[i].gravity.x * elapsedTime,  particles[i].gravity.y * elapsedTime);
        //Rotation:
             particles[i].shape.rotate( particles[i].rotationSpeed * elapsedTime);
        //Color:
             particles[i].colorProgress +=  particles[i].colorSpeed *elapsedTime;
            while ( particles[i].colorProgress > 255){
                 particles[i].colorProgress -= 255;
                 particles[i].nextColor = ( particles[i].nextColor+1)%6;                
            }
        //Transparency:
             particles[i].alpha -=  particles[i].fadeoutSpeed *elapsedTime;
             if ( particles[i].alpha < 0){  particles[i].alpha = 0; }
             particles[i].shape.setFillColor(ColorAnimator::getInterpolatedColor( particles[i].nextColor,  particles[i].colorProgress, static_cast<sf::Uint8>( particles[i].alpha)));
        //Delete if invisible:
            sf::Vector2f pos =  particles[i].shape.getPosition();
            if( particles[i].alpha <= 0 || 
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
