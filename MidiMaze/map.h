#pragma once
#include <mesh.h>
#include <wall.h>

class Map
{
public:
    // model data 
    vector<Wall> map;
    int size;
    float dim = 1.5;
    vector<vector<bool>> laberinto;

    // constructor, expects a filepath to a 3D model.
    Map(string const& path, Shader& ourShader) {
        loadWalls(path, ourShader);
    };

    Map() {};

    void loadWalls(string const& path, Shader& ourShader) {
        ifstream filein(path);
        if (filein.is_open()) {
            string line, lastLine;
            getline(filein, line);
            size = static_cast<int>(stof(line));
            float start = size * dim / 2;

            int j = 0;
            vector<bool> aux;
            getline(filein, line);
            for (int i = 0; i < size; i++) {
                if (line[i] == 'x') {
                    aux.push_back(true);
                    if (line[i + 1] == 'x') {
                        float x = -start + dim * i;
                        float z = start - dim * j;
                        map.push_back(Wall(glm::vec3(x, -0.5, z), glm::vec3(x + dim, -0.5, z), ourShader));
                    }
                    else {
                        aux.pop_back();
                        aux.push_back(false);
                    }
                }
                else {
                    aux.push_back(false);
                }
            }
            laberinto.push_back(aux);
            j++;
            lastLine = line;


            while (getline(filein, line)) {
                //cout << line << endl;
                vector<bool> aux;
                for (int i = 0; i < size; i++) {
                    if (line[i] == 'x') {
                        aux.push_back(true);
                        if (line[i + 1] == 'x') {
                            float x = -start + dim * i;
                            float z = start - dim * j;
                            map.push_back(Wall(glm::vec3(x, -0.5, z), glm::vec3(x + dim, -0.5, z), ourShader));
                        }
                        if (lastLine[i] == 'x') {
                            float x = -start + dim * i;
                            float z = start - dim * (j - 1);
                            map.push_back(Wall(glm::vec3(x, -0.5, z), glm::vec3(x, -0.5, z - dim), ourShader));
                        }
                    }
                    else {
                        aux.push_back(false);
                    }
                }
                j++;
                lastLine = line;
                laberinto.push_back(aux);
            }
            if (j < size || j>size) {
                cout << "ERROR: Map dimensions might be wrong.";
                exit(0);
            }

            for (int i = 1; i < laberinto.size() - 1; i++) {
                for (int j = 1; j < laberinto[i].size() - 1; j++) {
                    if (laberinto[i][j]) {
                        if (!laberinto[i][j - 1] && !laberinto[i][j + 1] && !laberinto[i - 1][j] && !laberinto[i + 1][j]) {
                            laberinto[i][j] = false;
                        }
                    }
                }
            }
        }
        else {
            cerr << "Error abriendo el archivo" << endl;
        }
    }
    vector<vector<bool>> getLab() {
        return laberinto;
    }

    float getDim() {
        return dim;
    }
    
    bool checkIntersections(glm::vec3 camera, glm::vec3 newCamera) {
        bool intersects = false;
        for (unsigned int i = 0; i < map.size(); i++) {
            intersects = map[i].intersect(camera.x, camera.z, newCamera.x, newCamera.z);
            if (intersects) {
                return true;
            }
        }
        return false;
    }

    bool checkCollisionBullets(glm::vec3 bulletPos, float radius) {
        bool intersects = false;
        for (unsigned int i = 0; i < map.size(); i++) {
            intersects = map[i].intersectsBullets(bulletPos, radius);
            if (intersects) {
                return true;
            }
        }
        return false;
    }

    void Draw(Shader& shader)
    {
        for (unsigned int i = 0; i < map.size(); i++)
            map[i].draw(shader);
    }
};