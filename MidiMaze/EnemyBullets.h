#pragma once
#include <model.h>
#include <map.h>
#include <Enemies.h>

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
		if (distance < 0.4f) {
			return true;
        }
        else {
			return false;
        }
    }

    bool checkCollisionWithEnemies(int bulletIndex, float radious_enemy, const vector<glm::vec3>* positions_Enemy, int enemyIndex) {
        for (int i = 0; i < positions_Enemy->size(); i++) {
            if (enemyIndex != i) {
                glm::vec3 vec = positions_Enemy->at(i) - positions[bulletIndex];

                float longit = static_cast<float>(sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2)));


                if (longit < (radious_enemy + radious)) {
                    //cout << vec.x << " " << vec.y << " " << vec.z << endl;
                    //cout << longit << "  ----  " << radious << " " << radiousBullet << endl;
                    return true;
                }
            }
        }
        return false;
    }

    int DrawBullets(Shader& shader, Map mapa, float deltaTime, glm::vec3 playerPosition, float radious_Enemy, const vector<glm::vec3> *positions_Enemy, int index) {
        int balasAcertadas = 0;
        for (int i = 0; i < numBullets; i++) {
            if (!collided[i]) {
                collided[i] = mapa.checkCollisionBullets(positions[i], positions[i] + directions[i] * bulletSpeed*deltaTime,radious);
                if (!collided[i]) {
                    collided[i] = mapa.checkCollisionBullets(positions[i], positions[i] + directions[i] * bulletSpeed * deltaTime, radious) 
                        || checkCollisionWithPlayer(i, playerPosition) || checkCollisionWithEnemies(i, radious_Enemy, positions_Enemy, index);
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