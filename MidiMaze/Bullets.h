#pragma once
#include <model.h>

class Bullet
{
public:
    // model data 
    Model bullet;
    int numBullets;
    vector<glm::vec3> positions;
    vector<glm::vec3> directions;
    float scale;

    // constructor, expects a filepath to a 3D model.
    Bullet(string const& path, float scale) : scale(scale) {
        Model a(path);
        bullet = a;
        numBullets = 0;
    };

    void newBullet(glm::vec3 position, glm::vec3 direction) {
        numBullets++;
        positions.push_back(position);
        directions.push_back(glm::vec3(direction[0]/2.3,direction[1]/2.3,direction[2]/2.3));
    }

    void DrawBullets(Shader& shader) {
        for (unsigned int i = 0; i < numBullets; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            positions[i] = positions[i] + directions[i];
            model = glm::translate(model, positions[i]); // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(scale,scale,scale));
            shader.setMat4("model", model);
            bullet.Draw(shader);
        }
    }


};