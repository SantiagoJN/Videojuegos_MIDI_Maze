#pragma once
#include <model.h>
#include <map.h>

class EnemBullet
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
    EnemBullet(string const& path, float scale) : scale(scale) {
        Model a(path);
        bullet = a;

        radious = bullet.getRadious() * scale/2;

        numBullets = 0;
    };


    void newBullet(glm::vec3 position, glm::vec3 direction) {
        numBullets++;
        positions.push_back(position);
        directions.push_back(glm::vec3(direction[0],direction[1],direction[2]));
        collided.push_back(false);
    }
	
    bool checkCollisionWithPlayer(int bulletIndex, glm::vec3 playerPosition) {
		//Simplemente se comprueba si está lo suficientemente cerca del jugador
		float distance = glm::distance(positions[bulletIndex], playerPosition);
		if (distance < 0.2f) {
			return true;
        }
        else {
			return false;
        }
    }

    int DrawBullets(Shader& shader, Map mapa, float deltaTime, glm::vec3 playerPosition) {
        int balasAcertadas = 0;
        for (int i = 0; i < numBullets; i++) {
            if (!collided[i]) {
                collided[i] = mapa.checkCollisionBullets(positions[i], positions[i] + directions[i] * bulletSpeed*deltaTime,radious);
                if (!collided[i]) {
                    collided[i] = checkCollisionWithPlayer(i, playerPosition);
                    if (!collided[i]) {
                        glm::mat4 model = glm::mat4(1.0f);
                        positions[i] = positions[i] + directions[i] * bulletSpeed * deltaTime;
                        model = glm::translate(model, positions[i]);
                        model = glm::scale(model, glm::vec3(scale, scale, scale));
                        shader.setMat4("model", model);
                        bullet.Draw(shader);
                    }
                    else {
                        balasAcertadas++;
                    }
                }
                
            }
        }
        return balasAcertadas;
    }


};