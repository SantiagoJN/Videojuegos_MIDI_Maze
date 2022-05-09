#pragma once
#include <EnemyBullets.h>
#include <model.h>
#include <stdlib.h>
#include <time.h>
#include <map.h>
#include <iostream>


#include <irrKlang/irrKlang.h>
using namespace irrklang;

ISoundEngine* SoundEngine = createIrrKlangDevice(); // to manage the sound effects

string basePath = "resources/objects/smileys/";
string colors[] = {"blue", "orange", "green", "pink", "lightblue", "magenta", "white", 
                   "yellow2", "blue2", "orange2", "green2", "pink2", "lightblue2", "magenta2", "white2"};
string bulletColors[] = { "blue", "orange", "green", "pink", "lightblue", "magenta", "white",
                   "yellow", "blue", "orange", "green", "pink", "lightblue", "magenta", "white" };
string yellowColor = "yellow";

enum Estados{
    PARADO,
    GIRANDO,
    ANDANDO,
	APUNTANDO
};

/*  COLORES EN HEXA
    Amarillo: e4f812
	Azul: 4c3de3
	Naranja: f58c11
	Verde: 03d10a
	Rosa: ef0ce5
	Azul claro: 15aee7
	Magenta: 8e0b5a
	Blanco: f5f0f4
*/

enum cadencia { CAD_RAPIDA, CAD_LENTA }; // Rapida = 1s, lenta = 3s
int reloadTime[] = { 1,3 }; // Delays para representar la cadencia de disparo. Cuanto más delay, menos cadencia

enum nivelesDificultad { VERY_DUMB, PLAIN_DUMB, NOT_SO_DUMB};

enum velocidadRegeneracion {REG_RAPIDA, REG_LENTA};
int regenJugador[] = { 5, 10 }; // Numero de frames que se regenera el jugador
bool regenSeleccionada;

int hit_time = 50; // Numero de frames que un enemigo se pone amarillo al golpearlo
float angulo_vision = 45.0f; // 90 grados en total

class Enemy
{
private:
    // model data
    vector<Model> enemyArray;
    Model yellowModel;
    //Model enemy;
    int numEnemies;
    vector<glm::vec3> positions;
    vector<glm::vec3> directions;
    vector<float> currentRotation; // Angulo actual de rotacion
    vector<float> goalRotation; // Hasta qué ángulo se quiere rotar
    vector<float> prevGoalRotation; // Para no perder el ángulo al girar
    vector<Estados> prevState; // Estado anterior al apuntar
    vector<unsigned int> currentDelays;
    vector<cadencia> cadencias; // Cadencias de los enemigos
    //EnemBullet bala("resources/objects/bullets/yellow/yellow.obj", 0.1);
    //EnemBullet myBullets("resources/objects/bullets/yellow/yellow.obj", 0.1);

    vector<EnemBullet> bullets; // Modelos para las balas de los enemigos

    vector<bool> viendo; // Si está viendo al jugador o no
    vector<Estados> states; // Estado del enemigo i: {ANDANDO, GIRANDO, PARADO}
    vector<nivelesDificultad> dificultades; // Dificultad de cada enemigo
    vector<glm::vec2> prevIndex;
    vector<glm::vec2> index;
    vector<glm::vec3> destiny;

    
    int counterRegen = 0;

    float radious;

    Map mapa;

    int num_vidas;

    vector<int> vidas;
    vector<int> puntuaciones;
    vector<int> hit_timeout; // Vector para dibujar los enemigos 
    int puntuacionJugador;
    float enemySpeed = 2.0f;
    float rotationSpeed = 120.0f;

    vector<vector<bool>> map;
	vector<vector<bool>> spawnmap;
    float scale;
    float dim;

public:
    

    vector<glm::vec3> returnPositions() {
        return positions;
    }

    Enemy() {};

    // constructor, expects a filepath to a 3D model.
    Enemy(float scale,int numvidas, int nDumbs, int nMDumbs, int nNDumbs, vector<vector<bool>> laberinto, Map mapa, 
        float dim, bool regenSpeed, bool reloadSpeed) 
        : mapa(mapa), map(laberinto), dim(dim), scale(scale){
        // Comprobamos que el n�mero de enemigos es correcto
        regenSeleccionada = regenSpeed;
        num_vidas = numvidas;
        numEnemies = nDumbs + nMDumbs + nNDumbs;
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
        vector<vector<bool>> spawn(map);
        spawnmap = vector<vector<bool>>(map);
//        bullets.reserve(numEnemies);
        for (int enemy = 0; enemy < numEnemies; enemy++) {
            bool end = false;
            int random = rand() % (spawn.size()-2) +1;
            float x, z;
            for (int i = random; i < spawn.size()-1; i++) {
                for (int j = random; j < spawn[i].size()-1; j++) {
                    if (!spawn[i][j] && (!spawn[i+1][j] || !spawn[i-1][j] || !spawn[i][j+1] || !spawn[i][j-1])) { // El enemigo se puede colocar en i,j
                        end = true;
                        //cout << i << " " << j << endl;
                        x = -start + j * dim;
                        z = start - i * dim;
                        glm::vec3 dir(1,1,1);
                        spawn[i][j] = true;
                        prevIndex.push_back(glm::vec2(i, j));
                        glm::vec2 ind = nextIndex(i, j, dir);
                        destiny.push_back(glm::vec3(-start + ind.y * dim, 0, start - ind.x * dim));
                        index.push_back(ind);
                        //cout << "Enemigo " << enemy << endl;
                        //cout << "\tPosicion: " << x << ", " << z << endl;
                        glm::vec3 position(x, 0, z);
                        positions.push_back(position);
                        //cout << "\tDireccion: " << dir[0] << dir[1] << dir[2] << endl;
                        directions.push_back(dir);
                        vidas.push_back(num_vidas); // Vidas de los enemigos
                        states.push_back(ANDANDO);   //Estado inicial
                        prevState.push_back(ANDANDO); // Valor por defecto~~
                        currentDelays.push_back(0); // Delays de los disparos
						if(reloadSpeed) cadencias.push_back(CAD_RAPIDA); // Les ponemos cadencia
                        else cadencias.push_back(CAD_LENTA);
                        

                        if (enemy < nDumbs) dificultades.push_back(VERY_DUMB); // Dificultad del enemigo
                        else if (enemy < (nDumbs + nMDumbs))  dificultades.push_back(PLAIN_DUMB); // Dificultad del enemigo
                        else dificultades.push_back(NOT_SO_DUMB);

                        bullets.push_back(EnemBullet("resources/objects/bullets/" + bulletColors[enemy] + "/" + bulletColors[enemy] + ".obj", 0.1f));
                        viendo.push_back(false);   
                        prevGoalRotation.push_back(0.0f); 
                        currentRotation.push_back(0.0f); // Rotación inicial (se va a actualizar el primer frame)
                        puntuaciones.push_back(0);
                        hit_timeout.push_back(0);
                        break;
                    }
                }
                if (end) break;
                else if (i == spawn.size() - 2) i = 1;
            }
        }
        for (int i = 0; i < numEnemies; i++) {
            rotacionInicial(i); // Poner la rotación inicial de los enemigos
            goalRotation.push_back(0.0f); // Inicializar el vector para modificarlo más adelante
        }

    };

    void rotacionInicial(int enemyIndex) {
        // Girar las caras para que siempre miren hacia el frente
        if (directions[enemyIndex][2] != 0.0) { // Se est� moviendo en el eje Z
            if (directions[enemyIndex][2] > 0.0) { // Se mueve hacia el NORTE
                currentRotation[enemyIndex] = 0.0f;
            }
            else { // Se mueve hacia el SUR
                currentRotation[enemyIndex] = 180.0f;
            }
        }
        else { // Se mueve en el eje X
            if (directions[enemyIndex][0] > 0.0) { // Se mueve hacia el ESTE
                currentRotation[enemyIndex] = 270.0f;
            }
            else { // Se mueve hacia el OESTE
                currentRotation[enemyIndex] = 90.0f;
            }
        }
    }
	
    void updateGoalRotation(int enemyIndex) {
		// Actualizar el ángulo de rotación hasta el que queremos que se mueva
		if (directions[enemyIndex][2] != 0.0) { // Se est� moviendo en el eje Z
			if (directions[enemyIndex][2] > 0.0) { // Se mueve hacia el NORTE
				goalRotation[enemyIndex] = 0.0f;
			}
			else { // Se mueve hacia el SUR
				goalRotation[enemyIndex] = 180.0f;
			}
		}
		else { // Se mueve en el eje X
			if (directions[enemyIndex][0] > 0.0) { // Se mueve hacia el ESTE
				goalRotation[enemyIndex] = 270.0f;
			}
			else { // Se mueve hacia el OESTE
				goalRotation[enemyIndex] = 90.0f;
			}
		}
    }

    glm::vec2 nextIndex(int i, int j, glm::vec3& dir) {
        int wh = rand() % 4;
        //bool chosen = false;
        bool one = false;
        bool two = false;
        bool three = false;
        bool four = false;
        int times = 0;
        while (1) {
            switch (wh) {
            case 0:
                if (!map[i][j + 1]) {
                    if (dir.x != -1.0 || (four && two && three)) {
                        dir = glm::vec3(1.0, 0, 0);
                        //map[i][j] = false;
                        map[i][j+1] = true;
                        return glm::vec2(i, j + 1);
                    }
                }
                else {
                    one = true;
                }
            case 1:
                if (!map[i][j - 1]) {
                    if (dir.x != 1.0 || (one && four && three)) {
                        dir = glm::vec3(-1.0, 0, 0);
                        //map[i][j] = false;
                        map[i][j - 1] = true;
                        return glm::vec2(i, j - 1);
                    }
                }
                else {
                    two = true;
                }
            case 2:
                if (!map[i + 1][j]) {
                    if ((dir.z != 1.0) || (one && two && four)) {
                        dir = glm::vec3(0, 0, -1.0);
                        //map[i][j] = false;
                        map[i+1][j] = true;
                        return glm::vec2(i + 1, j);
                    }
                }
                else {
                    three = true;
                }
            case 3:
                if (!map[i - 1][j]) {
                    if ((dir.z != -1.0) || (one && two && three)) {
                        dir = glm::vec3(0, 0, 1.0);
                        //map[i][j] = false;
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
                    /*for (auto f : map) {
                        for (auto i : f) cout << i << " ";
                        cout << endl;
                    }
                    cout << "-----------------" << endl;*/
                    //cout << "No sense" << endl;
                    
                    dir = glm::vec3(0, 0, 0);
                    return glm::vec2(i, j);
                }
                if (times > 6)return glm::vec2(0, 0);
                times++;
            }
        }
        return glm::vec2(-1, -1); // Valor err�neo (se sale del while)
    }


    bool checkCollision(glm::vec3 positionBullet, float radiousBullet) {
        for (int enemy = 0; enemy < numEnemies; enemy++) {
            if (vidas[enemy] > 0) {
                glm::vec3 vec = positions[enemy] - positionBullet;
                
                float longit = static_cast<float>(sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2)));
                

                if (longit < (radious + radiousBullet)) {
                    //cout << vec.x << " " << vec.y << " " << vec.z << endl;
                    //cout << longit << "  ----  " << radious << " " << radiousBullet << endl;
                    vidas[enemy]--;
                    if (vidas[enemy] == 0) {
                        puntuacionJugador++;
                        if (puntuacionJugador == 10) {
                            cout << "*******Jugador gana!!******" << endl;
                        }
                        cout << "PUNTO!" << endl;
						// Reiniciamos el enemigo
                        map[static_cast<int>(prevIndex[enemy].x)][static_cast<int>(prevIndex[enemy].y)] = false;
                        map[static_cast<int>(index[enemy].x)][static_cast<int>(index[enemy].y)] = false;
                        vidas[enemy] = num_vidas;
                        bool spawned = false;
                        bool end = false;
                        int random = rand() % (map.size() - 2) + 1;
                        for (int i = random; i < map.size() - 1; i++) {
                            for (int j = random; j < map[i].size() - 1; j++) {
                                if (!map[i][j] && (!spawnmap[i + 1][j] || !spawnmap[i - 1][j] || !spawnmap[i][j + 1] || !spawnmap[i][j - 1])) { // El enemigo se puede colocar en i,j
                                    end = true;
                                    float start = map.size() * dim / 2;
                                    float x2 = -start + j * dim;
                                    float z2 = start - i * dim;
                                    glm::vec3 dir(1, 1, 1);
                                    prevIndex[enemy] = glm::vec2(i, j);
                                    glm::vec2 ind = nextIndex(i, j, dir);
                                    destiny[enemy] = glm::vec3(-start + ind.y * dim, 0, start - ind.x * dim);
                                    index[enemy] = ind;
                                    //cout << "Enemigo " << enemy << endl;
                                    //cout << "\tPosicion: " << x << ", " << z << endl;
                                    glm::vec3 position(i, 0, j);
                                    positions[enemy] = position;
                                    //cout << "\tDireccion: " << dir[0] << dir[1] << dir[2] << endl;
                                    directions[enemy] = dir;
                                    vidas[enemy] = num_vidas; // Vidas de los enemigos
                                    states[enemy] = ANDANDO;   //Estado inicial
                                    prevState[enemy] = ANDANDO; // Valor por defecto~~


                                    viendo[enemy] = false;
                                    prevGoalRotation[enemy] = 0.0f;
                                    currentRotation[enemy] = 0.0f; // Rotación inicial (se va a actualizar el primer frame)
                                    hit_timeout[enemy] = 0;
                                    spawned = true;
                                    cout << i << "," << j << endl;
                                    break;
                                }
                            }
                            if (end) break;
                            else if (i == map.size() - 2) i = 1;
                        }
                        cout << "elijo: "<< endl;
                        
                    }
                    SoundEngine->play2D("resources/effects/hitmarker.mp3", false); //Play the sound without loop
                    hit_timeout[enemy] = hit_time;
                    return true;
                }
            }
        }
        return false;
    }

    void blinded() {
        for (auto b : viendo) b = false;
    }

    glm::vec2 getFreePosition() {
        bool spawned = false;
        int random = rand() % (map.size() - 2) + 1;
        bool end = false;
        for (int i = random; i < map.size() - 1; i++) {
            for (int j = random; j < map[i].size() - 1; j++) {
                if (!map[i][j] && (!spawnmap[i + 1][j] || !spawnmap[i - 1][j] || !spawnmap[i][j + 1] || !spawnmap[i][j - 1])) {
                    end = true;
                    float start = map.size() * dim / 2;
                    float x2 = -start + j * dim;
                    float z2 = start - i * dim;
                    return glm::vec2(x2, z2);
                }
            }
            if (end)break;
            else if (i == map.size() - 2) i = 1;
        }
        return glm::vec2(0,0);
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


    bool between(int enemyIndex, glm::vec3 A, glm::vec3 B) {
        glm::vec3 dir = glm::normalize(B - A);
        //if (enemyIndex == 0) cout << "call" << endl;
        for (int i = 0; i < numEnemies; i++) {
            if (enemyIndex != i) {
                glm::vec3 og_cent = A - positions[i];
                double b = 2.0 * dir.x * og_cent.x + 2.0 * dir.y * og_cent.y + 2.0 * dir.z * og_cent.z;
                double l = glm::length(og_cent);
                double c = l * l - pow(radious, 2);

                double intersect = b * b - 4 * c;

                double distance = 0;
                //cout << intersect << endl;
                if (intersect >= 0) {
                    double intA = (-b + sqrt(intersect)) / 2.0;
                    double intB = (-b - sqrt(intersect)) / 2.0;

                    /* if(intA < intB && intA>0.03) distance = intA;
                     else distance = intB;*/
                     //cout << intA << " " << intB << endl;
                    if (intA > 0.03 && intB > 0.03) {          //Podemos devolver A
                        if (intA < intB) distance = intA;
                        else distance = intB;
                    }
                    else if (intA > 0.03 && intB < 0.03) {
                        distance = intA;
                    }
                    else if (intA < 0.03 && intB>0.03) {
                        distance = intB;
                    }
                    else distance = intB;
                    //if(enemyIndex==0)cout <<"from"<< enemyIndex <<"to "<< i<< "\t" << distance << "\t\t" << glm::length(B - A) << endl;
                }

                if (intersect >= 0 && distance > 0 && distance < glm::length(B - A)) return true;
            }
        }
        return false;
    }



	// Función para actualizar el valor de la rotación del enemigo enemyIndex
    void actualizarRotacion(int enemyIndex, float deltaTime, glm::vec3 playerPosition) {
        // Si está apuntando el ángulo es similar al objetivo, no hacer nada
        if (states[enemyIndex] == APUNTANDO) {
            float angle = currentRotation[enemyIndex] - goalRotation[enemyIndex];
            if (angle < 0) angle += 360.0f; // Hacemos el módulo
            if (angle < rotationSpeed * deltaTime || angle > 360.0f - rotationSpeed * deltaTime) return;
        }
        float diff = currentRotation[enemyIndex] - goalRotation[enemyIndex];
		if (diff>220.0f || (-220<diff && diff<0)) {
			currentRotation[enemyIndex] += rotationSpeed * deltaTime;
            float aux = currentRotation[enemyIndex];
            if (currentRotation[enemyIndex] > 360.0f) currentRotation[enemyIndex] -= 360.0f;
            bool andar = false;

            if (360.0f - currentRotation[enemyIndex] < rotationSpeed * deltaTime) currentRotation[enemyIndex] = 0.0f;
            
            if (aux < goalRotation[enemyIndex]) andar = currentRotation[enemyIndex] >= goalRotation[enemyIndex];
            else andar = currentRotation[enemyIndex] < 10;

            // Si la rotación actual es cercana a 360º, se pone a 0
            if ( andar && states[enemyIndex] == GIRANDO) {
                //cout << prevIndex[enemyIndex].x << " " << prevIndex[enemyIndex].y << "false" << endl;
                map[static_cast<unsigned int>(prevIndex[enemyIndex].x)][static_cast<unsigned int>(prevIndex[enemyIndex].y)] = false;
                states[enemyIndex] = ANDANDO;
            }
		}
		else{
			currentRotation[enemyIndex] -= rotationSpeed * deltaTime;
            
            if (currentRotation[enemyIndex] < 0.0f) currentRotation[enemyIndex] += 360.0f;
            if (currentRotation[enemyIndex] <= goalRotation[enemyIndex] && states[enemyIndex] == GIRANDO) {
                //cout << prevIndex[enemyIndex].x << " " << prevIndex[enemyIndex].y << "false" << endl;
                map[static_cast<unsigned int>(prevIndex[enemyIndex].x)][static_cast<unsigned int>(prevIndex[enemyIndex].y)] = false;
                states[enemyIndex] = ANDANDO;
            }
            //cout << "next rotation: " << currentRotation[enemyIndex] << ", goalRotation: " << goalRotation[enemyIndex] << endl;
		}
        if(dificultades[enemyIndex] != VERY_DUMB) actualizarViendo(enemyIndex, playerPosition, true);
    }

    // Código común que se repite en las funciones de actualización de estado del enemigo
    void pintarEnemigo(int enemyIndex, Shader& shader) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, positions[enemyIndex]);
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        //Model = glm::rotate(Model, angle_in_radians, glm::vec3(x, y, z)); // where x, y, z is axis of rotation (e.g. 0 1 0)				
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 0, 1)); // Poner las caras bien (problema con textura)
        model = glm::rotate(model, glm::radians(currentRotation[enemyIndex]), glm::vec3(0, 1, 0)); // Rotamos en la dirección en la que está mirando

        shader.setMat4("model", model);
        if (hit_timeout[enemyIndex] > 0) { // Dibujarlo amarillo un momento cuando se le golpea
            yellowModel.Draw(shader);
            hit_timeout[enemyIndex]--; // Ya hemos dibujado un frame
        }
        else {
            enemyArray[enemyIndex].Draw(shader);
        }
    }

	// Gestionar el movimiento cuando el enemigo está parado
    void gestionarParado(int enemyIndex, Shader& shader) {
    }


    bool pararAndar(int enemyIndex, float deltaTime) {
        float x = (positions[enemyIndex].x + directions[enemyIndex].x * deltaTime * enemySpeed);
        float z = (positions[enemyIndex].z + directions[enemyIndex].z * deltaTime * enemySpeed);
        if (directions[enemyIndex].x < 0) {
            return x < destiny[enemyIndex].x;
        }
        else if (directions[enemyIndex].x > 0) {
            return x > destiny[enemyIndex].x;
        }
        else if (directions[enemyIndex].z < 0) {
            return z < destiny[enemyIndex].z;
        }
        else if (directions[enemyIndex].z > 0) {
            return z > destiny[enemyIndex].z;
        }
        else return true;
    }

	
	// Gestionar el movimiento cuando el enemigo está andando
    void gestionarAndando(int enemyIndex, Shader& shader, float deltaTime, glm::vec3 playerPosition) {
		//Calculamos las actualizaciones necesarias
        float start = map.size() * dim / 2;
        //if ((positions[enemyIndex].x == destiny[enemyIndex].x) && (positions[enemyIndex].z == destiny[enemyIndex].z)) {
        if(pararAndar(enemyIndex, deltaTime)){
            positions[enemyIndex] = destiny[enemyIndex];
            glm::vec3 prevDir = directions[enemyIndex];
            prevIndex[enemyIndex] = index[enemyIndex];
            index[enemyIndex] = nextIndex(static_cast<int>(index[enemyIndex].x), static_cast<int>(index[enemyIndex].y), directions[enemyIndex]);
            if (prevDir != directions[enemyIndex]) {
                //cout << "Cambiando dir" << prevDir.x<< " " << prevDir.z << " --> " << directions[enemyIndex].x << " " << directions[enemyIndex].z << endl;
                updateGoalRotation(enemyIndex);
                states[enemyIndex] = GIRANDO;
            }
            else {
                map[static_cast<unsigned int>(prevIndex[enemyIndex].x)][static_cast<unsigned int>(prevIndex[enemyIndex].y)] = false;
            }
            destiny[enemyIndex] = glm::vec3(-start + index[enemyIndex].y * dim, 0, start - index[enemyIndex].x * dim);
            if (dificultades[enemyIndex] != VERY_DUMB) actualizarViendo(enemyIndex, playerPosition, true);
        }
        if (states[enemyIndex] == ANDANDO) { // Si seguimos moviéndonos
			//cout << "directions: " << directions[enemyIndex].x << " " << directions[enemyIndex].y << " " << directions[enemyIndex].z << endl;
            positions[enemyIndex].x = (positions[enemyIndex].x + directions[enemyIndex].x * deltaTime * enemySpeed);
            positions[enemyIndex].z = (positions[enemyIndex].z + directions[enemyIndex].z * deltaTime * enemySpeed);
            //cout << "positions = " << positions[enemyIndex].x << ", " << positions[enemyIndex].z << endl;
        }
        
    }

	// Gestionar el movimiento cuando el enemigo está girando
	void gestionarGirando(int enemyIndex, Shader& shader, float deltaTime, glm::vec3 playerPosition) {
        actualizarRotacion(enemyIndex, deltaTime, playerPosition); // Rotar lo que sea necesario
		
	}

    void disparaEnemigo(int enemyIndex, float deltaTime, glm::vec3 playerPosition) {
        if (currentDelays[enemyIndex] == 0) {
            glm::vec3 dirDisparo = glm::normalize(playerPosition - positions[enemyIndex]);
			//glm::vec3 dirDisparo = glm::vec3(cos(currentRotation[enemyIndex]), 0, sin(currentRotation[enemyIndex]));
            //cout << currentRotation[enemyIndex] << " -> " << dirDisparo.x << ", " << dirDisparo.z << endl;
            // Miramos si hay que desviar el disparo
            float random = (rand() - (RAND_MAX/2)) / (float)(RAND_MAX*15); // Generamos un número entre {-1, 1}
            float random2 = (rand() - (RAND_MAX / 2)) / (float)(RAND_MAX*15); // Generamos otro
            /*if (dificultades[enemyIndex] == VERY_DUMB) {
                dirDisparo.x += 2 * random; // Lo movemos bastante
                dirDisparo.z += 2 * random2;
            }
            else */if (dificultades[enemyIndex] == PLAIN_DUMB) {
                dirDisparo.x += 2 * random; // Lo movemos un poco menos
                dirDisparo.z += 2 * random2; 
            }
			// Si es NOT_SO_DUMB, no se desvía nada
            bullets[enemyIndex].newBullet(positions[enemyIndex], dirDisparo);
            currentDelays[enemyIndex] = static_cast<unsigned int>(reloadTime[cadencias[enemyIndex]] / deltaTime);
            SoundEngine->play2D("resources/effects/disparo.mp3", false);			
            //cout << currentDelays[enemyIndex] << endl;
        }
    }

    int cont = 0;
    // Gestionar el movimiento cuando el enemigo está girando
    void gestionarApuntando(int enemyIndex, Shader& shader, glm::vec3 playerPosition, float deltaTime) {
        if (dificultades[enemyIndex] != VERY_DUMB) {
            // Vector que va desde el enemigo al jugador
            glm::vec3 enemyToPlayerVec = playerPosition - positions[enemyIndex];
            float deg = -atan(enemyToPlayerVec[0] / enemyToPlayerVec[2]) * 180.0f / 3.1415f;
            //if (deg < 0.0) deg = 180.0f - deg;
            if (playerPosition.z < positions[enemyIndex].z) {
                deg = 180.0f + deg;
            }
            if (deg < 0.0) deg = 360.0f + deg;

            /*
            cont++;
            if (cont == 200) {
                cout << "Jugador: " << playerPosition.x << ", " << playerPosition.z <<
                    ". Enemigo: " << positions[enemyIndex].x << ", " << positions[enemyIndex].z << ". Angulo: " << deg << endl;
                cont = 0;
            }*/

            goalRotation[enemyIndex] = deg;
            actualizarRotacion(enemyIndex, deltaTime, playerPosition); // Rotar lo que sea necesario
            if (abs(currentRotation[enemyIndex] - goalRotation[enemyIndex]) < rotationSpeed * deltaTime) {
                //Si forma un ángulo lo suficientemente pequeño, es que ya le está apuntando
                disparaEnemigo(enemyIndex, deltaTime, playerPosition);
            }
            
        }
    }
	
    void actualizarViendo(int enemyIndex, glm::vec3 playerPosition, bool quieto) {
        if (dificultades[enemyIndex] != VERY_DUMB) {
            // Sacamos el ángulo que forman
            glm::vec3 enemyToPlayerVec = playerPosition - positions[enemyIndex];
            float deg = -atan(enemyToPlayerVec[0] / enemyToPlayerVec[2]) * 180.0f / 3.1415f;
            if (playerPosition.z < positions[enemyIndex].z) deg = 180.0f + deg;
            if (deg < 0.0) deg = 360.0f + deg;

            float diferencia = abs(deg - currentRotation[enemyIndex]);
            //cout << "diferencia: " << diferencia << "; deg: " << deg << ", curr: " << currentRotation[enemyIndex] << endl;
            float distance = glm::length(playerPosition - positions[enemyIndex]);
            bool somethingBetween = mapa.wallBetween(positions[enemyIndex], playerPosition) || between(enemyIndex, positions[enemyIndex], playerPosition);
            if ((diferencia < angulo_vision || 360.0f - angulo_vision < diferencia) && !somethingBetween) { // Le está viendo
                // Si le está viendo, calculamos si hay alguna pared en medio

                if (states[enemyIndex] != APUNTANDO && quieto) {
                    //cout << "Guardando... Estado actual: " << states[enemyIndex] << ", rotación actual: " << goalRotation[enemyIndex] << endl;
                    prevState[enemyIndex] = states[enemyIndex];
                    prevGoalRotation[enemyIndex] = goalRotation[enemyIndex];
                    states[enemyIndex] = APUNTANDO;
                }
            }
            else { // No le está viendo
                if (states[enemyIndex] == APUNTANDO) { // Si antes le estaba apuntando, hay que recuperar el estado
                    if (prevState[enemyIndex] == GIRANDO) { // Si antes estaba girando, recuperamos el giro
                        //updateGoalRotation(enemyIndex);
                        goalRotation[enemyIndex] = prevGoalRotation[enemyIndex];
                    }
                    else if (prevState[enemyIndex] == ANDANDO) { // Si antes estaba andando, tendrá que corregir el giro
                        updateGoalRotation(enemyIndex);
                    }
                    states[enemyIndex] = GIRANDO;
                }
            }
        }
    }

    void actualizarDelay(int enemyIndex) {
        if (currentDelays[enemyIndex] > 0) currentDelays[enemyIndex]--;
    }

    void actualizarRegenJugador(int& vidasJugador, float deltaTime) {
        if (vidasJugador < num_vidas) {
            if (counterRegen == 1) { // Se le suma una vida
                vidasJugador++;
                SoundEngine->play2D("resources/effects/iniciopartida.mp3", false);
                if (vidasJugador < num_vidas) { // Si aún se puede recuperar más, se resetea el contador
                    if (regenSeleccionada) { // Regeneración lenta
                        counterRegen = static_cast<int>(static_cast<float>(regenJugador[REG_LENTA]) / deltaTime);
                    }
                    else {
                        counterRegen = static_cast<int>(static_cast<float>(regenJugador[REG_RAPIDA]) / deltaTime);
                    }
                }
                else {
                    counterRegen = 0;
                }
            }
            else if (counterRegen > 0) {
                //cout << "counterregen " << counterRegen << endl;
                counterRegen--; // Se actualiza el contador
            }
        }
    }
	
    void DrawEnemies(Shader& shader, glm::vec3 playerPosition, Enemy& enemies, Map mapa, float deltaTime, int& balasRecibidas, int& vidasJugador, bool pause) {
        hit_time = static_cast<int>(0.1 / deltaTime);
        int balasAcertadas = 0;
        int balasEnemigo = 0;
        for (int i = 0; i < numEnemies; i++) {
            balasEnemigo = bullets[i].DrawBullets(shader, mapa, deltaTime, playerPosition, radious, &positions, i);
            if (!pause) {
                if (vidas[i] > 0) {
                    if (dificultades[i] != VERY_DUMB) actualizarViendo(i, playerPosition, false); // Actualizar si el enemigo i me está viendo o no
                    actualizarDelay(i); // Actualizamos el contador del enemigo
                    //cout << "Enemigo " << i << ": " << states[i] << endl;
                    switch (states[i]) {
                    case PARADO: {gestionarParado(i, shader); break; }
                    case ANDANDO: {gestionarAndando(i, shader, deltaTime, playerPosition); break; }
                    case GIRANDO: {gestionarGirando(i, shader, deltaTime, playerPosition); break; }
                    case APUNTANDO: {gestionarApuntando(i, shader, playerPosition, deltaTime); break; }
                    default: {cerr << "Error en el estado del enemigo " << i << endl; break; }
                    }

                }
                if (balasEnemigo > 0) {
                    balasAcertadas += balasEnemigo;
                    vidasJugador -= balasEnemigo;
                    //cout << "Vidas: " << vidasJugador << endl;
                    if (vidasJugador <= 0) {
                        puntuaciones[i]++;
                        if (puntuaciones[i] == 10) {
                            cout << "******GANA ENEMIGO " << i << "******" << endl;
                        }
                    }
                    else{ // Si aún le quedan vidas y no tiene contador, se le da tiempo de recuperación
                        if (regenSeleccionada) { // Regeneración lenta
                            counterRegen = static_cast<int>(static_cast<float>(regenJugador[REG_LENTA]) / deltaTime);
                        }
                        else {
                            counterRegen = static_cast<int>(static_cast<float>(regenJugador[REG_RAPIDA]) / deltaTime);
                        }
                    }
                }
				
            }
            pintarEnemigo(i, shader); // Pintar el enemigo como tal
            actualizarRegenJugador(vidasJugador, deltaTime);
            
        }
        
        if (balasAcertadas > 0) SoundEngine->play2D("resources/effects/hitmarker.mp3", false);
        balasRecibidas = balasAcertadas;
    };

};