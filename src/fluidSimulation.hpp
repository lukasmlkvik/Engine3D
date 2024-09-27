#pragma once

#include "sphere.hpp"
#include "transform.hpp"
#include "shader.hpp"
#include <cstdlib>

const glm::vec3 vec_down = {0,-1,0};

const float REST_DENSITY = 100.0f;  // Hustota v pokoji
const float GAS_CONSTANT = 100.0f;  // Plynová konštanta pre tlak
const float MASS = 1.f;            // Hmotnosť častice
const float VISCOSITY = 100.0f;      // Viskozita
const float PARTICLE_RADIUS = 1.f;  // Polomer častice
const float GRAVITY = 9.81f;        // Gravitačná sila

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
    float pressure;
    glm::vec3 velocity;
    glm::vec3 predictedPos;


    // Kernel funkcia pre hustotu (Poly6 kernel)
    float kernelPoly6(float r) const {
        float h = PARTICLE_RADIUS;
        if (r >= 0 && r <= h) {
            float x = (h * h - r * r);
            return (315.0f / (64.0f * glm::pi<float>() * glm::pow(h, 9))) * x * x * x;
        } else {
            return 0.0f;
        }
    }

    void computeDensitiesAndPressures(std::vector<Particle>& particles) {
        density = 0.0f;
            for (const auto& p2 : particles) {
                float dist = glm::length(transform.position - p2.transform.position);
                if (dist < PARTICLE_RADIUS) {
                    density += MASS * kernelPoly6(dist);
                }
            }
            pressure = GAS_CONSTANT * (density - REST_DENSITY);
        
    }

    // Kernel funkcia pre tlak (Spiky kernel)
    float kernelSpiky(float r) const {
        float h = PARTICLE_RADIUS;
        if (r >= 0 && r <= h) {
            float x = (h - r);
            return (-45.0f / (glm::pi<float>() * glm::pow(h, 6))) * x * x;
        } else {
            return 0.0f;
        }
    }

    // Kernel funkcia pre viskozitu (Viscosity kernel)
    float kernelViscosity(float r) const {
        float h = PARTICLE_RADIUS;
        if (r >= 0 && r <= h) {
            return (45.0f / (glm::pi<float>() * glm::pow(h, 6))) * (h - r);
        } else {
            return 0.0f;
        }
    }

    void computeForces(std::vector<Particle>& particles, float dt) {
            glm::vec3 pressureForce(0, 0,0);
            glm::vec3 viscosityForce(0, 0,0);
            for (const auto& p2 : particles) {
                if (this == &p2) continue;

                float dist = glm::length(transform.position - p2.transform.position);
                if (dist < PARTICLE_RADIUS) {
                    // Tlaková sila
                    pressureForce = pressureForce - (p2.transform.position - transform.position) * (MASS * (pressure + p2.pressure) / (2 * p2.density) * kernelSpiky(dist));

                    // Viskózna sila
                    viscosityForce = viscosityForce + (p2.velocity - velocity) * (VISCOSITY * MASS / p2.density * kernelViscosity(dist));
                }
            }

            // Výsledná sila
            glm::vec3 force = pressureForce + viscosityForce + vec_down * GRAVITY;

            // Aplikovanie sily na zrýchlenie
            velocity = velocity + (force * (dt / density));
        
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
glm::vec3 Particle::boundarySize = {30,30,5};
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
            p.transform.position.z = rand()*0.0000001f;
            c++;
        }
    }

    void update(float dt){
        for(auto& p : particles){
            p.computeDensitiesAndPressures(particles);
        }
        for(auto& p : particles){
            p.computeForces(particles,dt);
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