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

    float radious;

    vector<bool> collided;
    float scale;

    // constructor, expects a filepath to a 3D model.
    Bullet(string const& path, float scale) : scale(scale) {
        Model a(path);
        bullet = a;

        radious = bullet.getRadious() * scale/2;

        numBullets = 0;
    };

    void newBullet(glm::vec3 position, glm::vec3 direction) {
        numBullets++;
        positions.push_back(position);
        directions.push_back(glm::vec3(direction[0]/2.3,direction[1]/2.3,direction[2]/2.3));
        collided.push_back(false);
    }

    void DrawBullets(Shader& shader, Enemy& enemies) {
        for (int i = 0; i < numBullets; i++) {
            if (!collided[i]) {
                collided[i] = mapa.checkCollisionBullets(positions[i], radious);
                if (!collided[i]) {
                    collided[i] = enemies.checkCollision(positions[i], radious);
                    if (!collided[i]) {
                        glm::mat4 model = glm::mat4(1.0f);
                        positions[i] = positions[i] + directions[i] / 50.0f;
                        model = glm::translate(model, positions[i]);
                        model = glm::scale(model, glm::vec3(scale, scale, scale));
                        shader.setMat4("model", model);
                        bullet.Draw(shader);
                    }
                }
            }
        }
    }


};