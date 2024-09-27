#pragma once

#include "sphere.hpp"
#include "transform.hpp"
#include "shader.hpp"
#include <cstdlib>

const double G = 6.67430e-11;

class Planet {
    public:

    static Mesh* mesh;
    static Shader* shader;
    static ui32 modelTransformID; 

    TransformComponent transform;
    glm::vec3 velocity;
    float mass;

    
    glm::vec3 color;


    void applyGravity(std::vector<Planet>& plants, float dt) {
        
        for (const auto& p2 : plants) {
            if(&p2 == this) continue;
            float dist = glm::length(transform.position - p2.transform.position);
            if (dist == 0) continue;
            float force = G * mass * p2.mass / (dist*dist);
            glm::vec3 force_v  = force * (p2.transform.position - transform.position) / dist;
            velocity += (force_v/mass) * dt;
        }

    }


    void update(float dt){
        // transform.position += glm::vec3((rand()%11)-5,(rand()%11)-5,(rand()%11)-5)*dt;
        transform.position += velocity * dt;
    }


    void draw(){
        shader->bind();
        glUniformMatrix4fv(modelTransformID, 1, GL_FALSE, glm::value_ptr(transform.mat4()));
        mesh->bind();
        mesh->draw();
    }
};
ui32 Planet::modelTransformID = 0;
Mesh* Planet::mesh = nullptr;
Shader* Planet::shader = nullptr;


class GravitySimulation {
public:
    std::vector<Planet> planets =std::vector<Planet>(2);

    GravitySimulation(){
        planets[0].mass = 5.972e24;
        
        planets[1].mass = 7.348e22;
        planets[1].transform.position = {384400000.f,0.f,0.f};
        planets[1].velocity = {0.f, 0.f, 1022.f};

        for(auto& p : planets){
            p.transform.scale *= glm::pow((3*p.mass)/(4* glm::pi<float>()* 1.0e-1f), 1.0f/3.0f);
            //std::cout <<p.transform.scale.x <<"\n";
        }

    }

    void update(float dt){
        for(auto& p : planets){
            p.applyGravity(planets,dt);
        }
        for(auto& p : planets){
            p.update(dt);
        }
    }
    
    void draw(){
        for(auto& p : planets){
            p.draw();
        }
    }
};