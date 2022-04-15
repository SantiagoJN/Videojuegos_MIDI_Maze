#pragma once
#include <model.h>
#include <stdlib.h>
#include <time.h>


#include <irrKlang/irrKlang.h>
using namespace irrklang;

ISoundEngine* SoundEngine = createIrrKlangDevice(); // to manage the sound effects

class Enemy
{
public:
    // model data 
    Model enemy;
    int numEnemies;
    vector<glm::vec3> positions;
    vector<glm::vec3> directions;
    vector<glm::vec2> index;
    vector<glm::vec3> destiny;

    float radious;

    vector<int> vidas;

    vector<vector<bool>> map;
    float scale;
    float dim;

    // constructor, expects a filepath to a 3D model.
    Enemy(string const& path, float scale, int numEnemies, vector<vector<bool>> laberinto, float dim) : numEnemies(numEnemies), map(laberinto), dim(dim), scale(scale) {
        Model a(path);
        enemy = a;

        radious = enemy.getRadious() * scale/2;

        srand(time(NULL));

        for (int i = 0; i < map.size(); i++) {
            for (int j = 0; j < map[i].size(); j++) {
                cout << map[i][j]<<" ";
            }
            cout << endl;
        }

        float start = map.size() * dim / 2;
        for (int i = 0; i < numEnemies; i++) {
            bool end = false;
            int random = rand() % (map.size()-2) +1;
            float x, z;
            for (int i = random; i < map.size(); i++) {
                for (int j = random; j < map[i].size(); j++) {
                    if (!map[i][j]) {
                        end = true;
                        cout << i << " " << j << endl;
                        x = -start + j * dim;
                        z = start - i * dim;
                        glm::vec3 dir(1,1,1);
                        map[i][j] = true;
                        glm::vec2 ind = nextIndex(i, j, dir);
                        destiny.push_back(glm::vec3(-start + ind.y * dim, 0, start - ind.x * dim));
                        index.push_back(ind);
                        cout << x << " " << z << endl;
                        glm::vec3 position(x, 0, z);
                        positions.push_back(position);
                        directions.push_back(dir);
                        vidas.push_back(1);
                        break;
                    }
                }
                if (end) break;
                else if (i == map.size() - 2) i = 1;
            }
        }

    };


    glm::vec2 nextIndex(int i, int j, glm::vec3& dir) {
        int wh = rand() % 4;
        bool chosen = false;
        bool one = false;
        bool two = false;
        bool three = false;
        bool four = false;
        while (!chosen) {
            switch (wh) {
            case 0:
                if (!map[i][j + 1]) {
                    if (dir.x != -0.05f || (four && two && three)) {
                        dir = glm::vec3(0.05, 0, 0);
                        map[i][j] = false;
                        map[i][j+1] = true;
                        return glm::vec2(i, j + 1);
                    }
                }
                else {
                    one = true;
                }
            case 1:
                if (!map[i][j - 1]) {
                    if (dir.x != 0.05f || (one && four && three)) {
                        dir = glm::vec3(-0.05, 0, 0);
                        map[i][j] = false;
                        map[i][j - 1] = true;
                        return glm::vec2(i, j - 1);
                    }
                }
                else {
                    two = true;
                }
            case 2:
                if (!map[i + 1][j]) {
                    if ((dir.z != 0.05f) || (one && two && four)) {
                        dir = glm::vec3(0, 0, -0.05);
                        map[i][j] = false;
                        map[i+1][j] = true;
                        return glm::vec2(i + 1, j);
                    }
                }
                else {
                    three = true;
                }
            case 3:
                if (!map[i - 1][j]) {
                    if ((dir.z != -0.05f) || (one && two && three)) {
                        dir = glm::vec3(0, 0, 0.05);
                        map[i][j] = false;
                        map[i-1][j] = true;
                        return glm::vec2(i - 1, j);
                    }
                }
                else {
                    four = true;
                }
            default:
                wh = 0;
                if (one && two && three && four) {
                    dir = glm::vec3(0, 0, 0);
                    return glm::vec2(i, j);
                }
            }
        }
    }


    bool checkCollision(glm::vec3 positionBullet, float radiousBullet) {
        for (int i = 0; i < numEnemies; i++) {
            if (vidas[i] > 0) {
                glm::vec3 vec = positions[i] - positionBullet;
                
                float longit = sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
                

                if (longit < (radious + radiousBullet)) {
                    cout << vec.x << " " << vec.y << " " << vec.z << endl;
                    cout << longit << "  ----  " << radious << " " << radiousBullet << endl;
                    vidas[i]--;
                    SoundEngine->play2D("resources/effects/hitmarker.mp3", false); //Play the sound without loop
                    return true;
                }
            }
        }
        return false;
    }


    void DrawEnemies(Shader& shader) {
        float start = map.size() * dim / 2;
        for (unsigned int i = 0; i < numEnemies; i++) {
            if (vidas[i] > 0) {
                glm::mat4 model = glm::mat4(1.0f);

                //cout << positions[i].x<< "," << positions[i].z << " -> " << destiny[i].x << "," << destiny[i].z << endl;
                //cout << directions[i].x << " " << directions[i].z<<"   ------     "<< index[i].x<<","<< index[i].y << endl;
                if ((positions[i].x == destiny[i].x) && (positions[i].z == destiny[i].z)) {
                    //cout << "hola\n\n\n\n" << endl;
                    index[i] = nextIndex(index[i].x, index[i].y, directions[i]);
                    destiny[i] = glm::vec3(-start + index[i].y * dim, 0, start - index[i].x * dim);
                }

                positions[i].x = roundf((positions[i].x + directions[i].x) * 100) / 100;
                positions[i].z = roundf((positions[i].z + directions[i].z) * 100) / 100;

                model = glm::translate(model, positions[i]);
                model = glm::scale(model, glm::vec3(scale, scale, scale));
                shader.setMat4("model", model);
                enemy.Draw(shader);
            }
        }
    };

};