#pragma once
#include <mesh.h>
#include <wall.h>

class Map
{
public:
    // model data 
    vector<Wall> map;
    int size;

    // constructor, expects a filepath to a 3D model.
    Map(string const& path, Shader& ourShader) {
        loadWalls(path, ourShader);
    };

    void loadWalls(string const& path, Shader& ourShader) {
        ifstream filein(path);
        if (filein.is_open()) {
            string line,lastLine;
            getline(filein, line);
            size = stoi(line);

            int j = 0;
            getline(filein, line);
            for (int i = 0; i < size; i++) {
                if (line[i] == 'x') {
                    if (line[i + 1] == 'x') {
                        float x = -size + 1.5 * i;
                        float z = size - 1.5 * j;
                        map.push_back(Wall(glm::vec3(x, -0.5, z), glm::vec3(x + 1.5, -0.5, z), ourShader));
                    }
                }
            }
            j++;
            lastLine = line;

            while (getline(filein, line)) {
                for (int i = 0; i < size; i++) {
                    if (line[i] == 'x') {
                        if (line[i + 1] == 'x'){
                            float x = -size + 1.5 * i;
                            float z = size - 1.5 * j;
                            map.push_back(Wall(glm::vec3(x, -0.5, z), glm::vec3(x + 1.5, -0.5, z), ourShader));
                        }
                        if (lastLine[i] == 'x') {
                            float x = -size + 1.5 * i;
                            float z = size - 1.5 * (j-1);
                            map.push_back(Wall(glm::vec3(x, -0.5, z), glm::vec3(x, -0.5, z -1.5), ourShader));
                        }
                    }
                }
                j++;
                lastLine = line;
            }
        }
        else {
            cerr << "Error abriendo el archivo" << endl;
        }
    }
    
    void Draw(Shader& shader)
    {
        for (unsigned int i = 0; i < map.size(); i++)
            map[i].draw(shader);
    }
};