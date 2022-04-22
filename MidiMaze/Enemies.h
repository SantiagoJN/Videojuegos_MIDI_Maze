#pragma once
#include <model.h>
#include <stdlib.h>
#include <time.h>


#include <irrKlang/irrKlang.h>
using namespace irrklang;

ISoundEngine* SoundEngine = createIrrKlangDevice(); // to manage the sound effects

string basePath = "resources/objects/smileys/";
string colors[] = {"blue", "orange", "green", "pink", "lightblue", "magenta", "white", 
                   "yellow2", "blue2", "orange2", "green2", "pink2", "lightblue2", "magenta2", "white2"};
string yellowColor = "yellow";

const int num_vidas = 3;
const int hit_time = 50; // Numero de frames que un enemigo se pone amarillo al golpearlo

class Enemy
{
public:
    // model data
    vector<Model> enemyArray;
    Model yellowModel;
    //Model enemy;
    int numEnemies;
    vector<glm::vec3> positions;
    vector<glm::vec3> directions;
    vector<glm::vec2> index;
    vector<glm::vec3> destiny;

    float radious;

    vector<int> vidas;
    vector<int> puntuaciones;
    vector<int> hit_timeout; // Vector para dibujar los enemigos 
    int puntuacionJugador;

    vector<vector<bool>> map;
    float scale;
    float dim;

    // constructor, expects a filepath to a 3D model.
    Enemy(float scale, int numEnemies, vector<vector<bool>> laberinto, float dim) : numEnemies(numEnemies), map(laberinto), dim(dim), scale(scale) {
        // Comprobamos que el n�mero de enemigos es correcto
        int numColors = sizeof(colors) / sizeof(colors[0]);
        assert(numEnemies <= numColors);

        // Cargamos los modelos de cada enemigo
        for (int i = 0; i < numEnemies; i++) {
            enemyArray.push_back(Model(basePath + colors[i] + "/" + colors[i] + ".obj"));
            //cout << "Metiendo " << colors[i] << endl;
        }
        yellowModel = Model(basePath + yellowColor + "/" + yellowColor + ".obj");

        radious = enemyArray[0].getRadious() * scale / 2; // Radio de los enemigos (para c�lculos)

        srand(static_cast<unsigned int>(time(NULL)));

        // Print del mapa
        /*for (int i = 0; i < map.size(); i++) {
            for (int j = 0; j < map[i].size(); j++) {
                cout << map[i][j]<<" ";
            }
            cout << endl;
        }*/

        // Colocamos los enemigos en el mapa de forma random
        float start = map.size() * dim / 2;
        for (int i = 0; i < numEnemies; i++) {
            bool end = false;
            int random = rand() % (map.size()-2) +1;
            float x, z;
            for (int i = random; i < map.size(); i++) {
                for (int j = random; j < map[i].size(); j++) {
                    if (!map[i][j]) { // El enemigo se puede colocar en i,j
                        end = true;
                        cout << i << " " << j << endl;
                        x = -start + j * dim;
                        z = start - i * dim;
                        glm::vec3 dir(1,1,1);
                        map[i][j] = true;
                        glm::vec2 ind = nextIndex(i, j, dir);
                        destiny.push_back(glm::vec3(-start + ind.y * dim, 0, start - ind.x * dim));
                        index.push_back(ind);
                        //cout << "Enemigo " << i << endl;
                        //cout << "\tPosicion: " << x << ", " << z << endl;
                        glm::vec3 position(x, 0, z);
                        positions.push_back(position);
                        //cout << "\tDireccion: " << dir[0] << dir[1] << dir[2] << endl;
                        directions.push_back(dir);
                        vidas.push_back(num_vidas);
                        puntuaciones.push_back(0);
                        hit_timeout.push_back(0);
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
        //bool chosen = false;
        bool one = false;
        bool two = false;
        bool three = false;
        bool four = false;
        while (1) {
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
        return glm::vec2(-1, -1); // Valor err�neo (se sale del while)
    }


    bool checkCollision(glm::vec3 positionBullet, float radiousBullet) {
        for (int i = 0; i < numEnemies; i++) {
            if (vidas[i] > 0) {
                glm::vec3 vec = positions[i] - positionBullet;
                
                float longit = static_cast<float>(sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2)));
                

                if (longit < (radious + radiousBullet)) {
                    cout << vec.x << " " << vec.y << " " << vec.z << endl;
                    cout << longit << "  ----  " << radious << " " << radiousBullet << endl;
                    vidas[i]--;
                    if (vidas[i] == 0) {
                        puntuacionJugador++;
                        cout << "PUNTO!" << endl;
                    }
                    SoundEngine->play2D("resources/effects/hitmarker.mp3", false); //Play the sound without loop
                    hit_timeout[i] = hit_time;
                    return true;
                }
            }
        }
        return false;
    }

    // Devuelve la puntuaci�n del jugador seguida del resto de puntuaciones
    vector<int> getScores() {
        vector<int> retVal;
        retVal.push_back(puntuacionJugador);
        for (int p : puntuaciones) {
            retVal.push_back(p);
        }
        return retVal;
    }


    void DrawEnemies(Shader& shader) {
        float start = map.size() * dim / 2;
        for (int i = 0; i < numEnemies; i++) {
            if (vidas[i] > 0) {
                glm::mat4 model = glm::mat4(1.0f);

                //cout << positions[i].x<< "," << positions[i].z << " -> " << destiny[i].x << "," << destiny[i].z << endl;
                //cout << directions[i].x << " " << directions[i].z<<"   ------     "<< index[i].x<<","<< index[i].y << endl;
                if ((positions[i].x == destiny[i].x) && (positions[i].z == destiny[i].z)) {
                    //cout << "hola\n\n\n\n" << endl;
                    index[i] = nextIndex(static_cast<int>(index[i].x), static_cast<int>(index[i].y), directions[i]);
                    destiny[i] = glm::vec3(-start + index[i].y * dim, 0, start - index[i].x * dim);
                }

                positions[i].x = roundf((positions[i].x + directions[i].x) * 100) / 100;
                positions[i].z = roundf((positions[i].z + directions[i].z) * 100) / 100;

                model = glm::translate(model, positions[i]);
                model = glm::scale(model, glm::vec3(scale, scale, scale));
                //Model = glm::rotate(Model, angle_in_radians, glm::vec3(x, y, z)); // where x, y, z is axis of rotation (e.g. 0 1 0)				
                model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 0, 1)); // Poner las caras bien (problema con textura)
                
                // Girar las caras para que siempre miren hacia el frente
                if (directions[i][2] != 0.0) { // Se est� moviendo en el eje Z
                    if (directions[i][2] > 0.0) { // Se mueve hacia el NORTE
                        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 1, 0));
                    }
                    else { // Se mueve hacia el SUR
                        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
                    }
                }
                else { // Se mueve en el eje X
                    if (directions[i][0] > 0.0) { // Se mueve hacia el ESTE
                        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0, 1, 0));
                    }
                    else { // Se mueve hacia el OESTE
                        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
                    }
                }
                shader.setMat4("model", model);
                if (hit_timeout[i] > 0) { // Dibujarlo amarillo un momento cuando se le golpea
                    yellowModel.Draw(shader);
                    hit_timeout[i]--; // Ya hemos dibujado un frame
                }
                else {
                    enemyArray[i].Draw(shader);
                }
                
            }
        }
    };

};