#pragma once
#include <model.h>
#include <Enemies.h>
#include <map.h>

class Bullet
{
public:
    // model data 
    Model bullet;
    int numBullets;
    vector<glm::vec3> positions;
    vector<glm::vec3> directions;
    
    float bulletSpeed = 12.0f;

    float radious;

    vector<bool> collided;
    float scale;

    // constructor, expects a filepath to a 3D model.
    Bullet( float scale) : scale(scale) {
        Model a("resources/objects/bullets/yellow/yellow.obj");
        bullet = a;

        radious = bullet.getRadious() * scale/2;

        numBullets = 0;
    };

    Bullet() {}

    void newBullet(glm::vec3 position, glm::vec3 direction) {
        numBullets++;
        positions.push_back(position);
        directions.push_back(glm::vec3(direction[0],direction[1],direction[2]));
        collided.push_back(false);
    }

    void DrawBullets(Shader& shader, Enemy& enemies, Map mapa, float deltaTime, bool pause, glm::vec3 playerPosition, bool& hayHit) {
        hayHit = false;
        if (!pause) {
            for (int i = 0; i < numBullets; i++) {
                if (!collided[i]) {
                    collided[i] = mapa.checkCollisionBullets(positions[i], positions[i] + directions[i] * bulletSpeed * deltaTime, radious * 0.8f);
                    if (!collided[i]) {
                        collided[i] = enemies.checkCollision(positions[i], radious, deltaTime, playerPosition);
                        if (!collided[i]) {
                            glm::mat4 model = glm::mat4(1.0f);
                            positions[i] = positions[i] + directions[i] * bulletSpeed * deltaTime;
                            model = glm::translate(model, positions[i]);
                            model = glm::scale(model, glm::vec3(scale, scale, scale));
                            shader.setMat4("model", model);
                            bullet.Draw(shader);
                        }
                        else {
                            hayHit = true;
                        }
                    }
                    
                }
            }
        }
    }


};