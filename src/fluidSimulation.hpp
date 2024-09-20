#pragma once

#include "sphere.hpp"
#include "transform.hpp"
#include "shader.hpp"
#include <cstdlib>

const glm::vec3 vec_down = {0,-1,0};

class Particle {
    public:

    static Mesh* mesh;
    static Shader* shader;
    static ui32 modelTransformID; 
    static glm::vec3 boundarySize; 
    static float colisionDunping; 
    static float gravity; 
    static float particleSize; 

    TransformComponent transform;
    float density; 
    glm::vec3 velocity;
    glm::vec3 predictedPos;




    
    float smoothingRadius = 1.2; 
    float mass = 1; 
    float targetDensity = 0.5; 
    float pressureMultiplier = 10; 

    float smoothingKernel(float dst, float radius){
        if(dst >= radius) return 0;
        float volume = glm::pi<float>() * glm::pow(radius, 8) / 4;
        float value = radius * radius - dst*dst;
        return value*value*value/volume;
    }

    float smoothingKernelDerivative(float dst, float radius){
        if(dst >= radius) return 0;
        float f = radius * radius - dst * dst;
        float scale = -24 / ( glm::pi<float>() * glm::pow(radius, 8));
        return scale * dst * f * f;
    }
    
    float convertDensityToPressure(float dens){
        float dE = dens - targetDensity;
        float pressure = dE * pressureMultiplier;
        return pressure;
    }

    float calculateDensity(std::vector<Particle>& particles){
       density = 0;
        for(auto& p : particles){
            float dst = glm::length(p.transform.position - predictedPos);
            float influence = smoothingKernel(dst, smoothingRadius);
            density += mass * influence;
        }
        return density;
    }

    glm::vec3 calculatePrressureForce(std::vector<Particle>& particles){
        glm::vec3 pF = {0,0,0};
        for(auto& p : particles){
           if(&p == this) continue;
           glm::vec3 offset = p.transform.position - transform.position;
           float dst = glm::length(offset);
           if (dst ==0.f){
                dst = 1;
           }
           glm::vec3 dir = offset/dst;

           float slope = smoothingKernelDerivative(dst, smoothingRadius);
           float d = p.density;
           float sharedPressure = (convertDensityToPressure(density) +convertDensityToPressure(d) )/2 ;

           pF += sharedPressure* dir * slope * mass / density;
        }
        return pF;
    }
    
    void applyPressureForce(std::vector<Particle>& particles, float dt){
        glm::vec3 presureForce = calculatePrressureForce(particles);
        glm::vec3 presureForceAcc = presureForce / density;
        velocity += (vec_down* gravity + presureForceAcc) * dt;
    }
    
    void predictPosition(float dt){
        predictedPos = transform.position + velocity / 120.f;
    }

    void update(float dt){
        // transform.position += glm::vec3((rand()%11)-5,(rand()%11)-5,(rand()%11)-5)*dt;
        transform.position += velocity * dt;
        resolveCollision();
    }

    void resolveCollision(){
        glm::vec3 halfBS = boundarySize * 0.5f - transform.scale;

        if(glm::abs(transform.position.x) > halfBS.x){
            transform.position.x = halfBS.x * (transform.position.x > 0 ? 1 : -1);
            velocity.x *= -1 * colisionDunping;
        }
        if(glm::abs(transform.position.y) > halfBS.y){
            transform.position.y = halfBS.y * (transform.position.y > 0 ? 1 : -1);
            velocity.y *= -1 * colisionDunping;
        }
        if(glm::abs(transform.position.z) > halfBS.z){
            transform.position.z = halfBS.z * (transform.position.z > 0 ? 1 : -1);
            velocity.z *= -1 * colisionDunping;
        }
    }

    void draw(){
        shader->bind();
        glUniformMatrix4fv(modelTransformID, 1, GL_FALSE, glm::value_ptr(transform.mat4()));
        mesh->bind();
        mesh->draw();
    }
};
ui32 Particle::modelTransformID = 0;
Mesh* Particle::mesh = nullptr;
Shader* Particle::shader = nullptr;
glm::vec3 Particle::boundarySize = {30,30,30};
float Particle::gravity = 9.f;
float Particle::colisionDunping = 0.8f;


class FluidSimulation {
public:
    std::vector<Particle> particles =std::vector<Particle>(400);

    FluidSimulation(){
        int c = 0;
        for(auto& p : particles){
            p.transform.position.x = c%20 - 10 + rand()*0.0000001f;
            p.transform.position.y = c/20 - 10 + rand()*0.0000001f;
            c++;
        }
    }

    void update(float dt){
        for(auto& p : particles){
            p.predictPosition(dt);
        }
        for(auto& p : particles){
            p.calculateDensity(particles);
        }
        for(auto& p : particles){
            p.applyPressureForce(particles,dt);
        }
        for(auto& p : particles){
            p.update(dt);
        }
    }
    
    void draw(){
        for(auto& p : particles){
            p.draw();
        }
    }
};