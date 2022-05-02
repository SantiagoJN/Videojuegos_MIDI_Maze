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

const int num_vidas = 3;
int hit_time = 50; // Numero de frames que un enemigo se pone amarillo al golpearlo

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
    vector<float> currentRotation; // Angulo actual de rotacion
    vector<float> goalRotation; // Hasta qué ángulo se quiere rotar
    vector<Estados> states; // Estado del enemigo i: {ANDANDO, GIRANDO, PARADO}
    vector<glm::vec2> prevIndex;
    vector<glm::vec2> index;
    vector<glm::vec3> destiny;

    float radious;

    vector<int> vidas;
    vector<int> puntuaciones;
    vector<int> hit_timeout; // Vector para dibujar los enemigos 
    int puntuacionJugador;
    float enemySpeed = 0.052f;
    float rotationSpeed = 120.0f;

    vector<vector<bool>> map;
    float scale;
    float dim;

    

    vector<glm::vec3> returnPositions() {
        return positions;
    }




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
        vector<vector<bool>> spawn(map);
        for (int i = 0; i < numEnemies; i++) {
            bool end = false;
            int random = rand() % (spawn.size()-2) +1;
            float x, z;
            for (int i = random; i < spawn.size(); i++) {
                for (int j = random; j < spawn[i].size(); j++) {
                    if (!spawn[i][j]) { // El enemigo se puede colocar en i,j
                        end = true;
                        cout << i << " " << j << endl;
                        x = -start + j * dim;
                        z = start - i * dim;
                        glm::vec3 dir(1,1,1);
                        spawn[i][j] = true;
                        prevIndex.push_back(glm::vec2(i, j));
                        glm::vec2 ind = nextIndex(i, j, dir);
                        destiny.push_back(glm::vec3(-start + ind.y * dim, 0, start - ind.x * dim));
                        index.push_back(ind);
                        //cout << "Enemigo " << i << endl;
                        //cout << "\tPosicion: " << x << ", " << z << endl;
                        glm::vec3 position(x, 0, z);
                        positions.push_back(position);
                        //cout << "\tDireccion: " << dir[0] << dir[1] << dir[2] << endl;
                        directions.push_back(dir);
                        vidas.push_back(num_vidas); // Vidas de los enemigos
                        states.push_back(APUNTANDO);   //Estado inicial
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
        while (1) {
            switch (wh) {
            case 0:
                if (!map[i][j + 1]) {
                    if (dir.x != -enemySpeed || (four && two && three)) {
                        dir = glm::vec3(enemySpeed, 0, 0);
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
                    if (dir.x != enemySpeed || (one && four && three)) {
                        dir = glm::vec3(-enemySpeed, 0, 0);
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
                    if ((dir.z != enemySpeed) || (one && two && four)) {
                        dir = glm::vec3(0, 0, -enemySpeed);
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
                    if ((dir.z != -enemySpeed) || (one && two && three)) {
                        dir = glm::vec3(0, 0, enemySpeed);
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
                    //cout << vec.x << " " << vec.y << " " << vec.z << endl;
                    //cout << longit << "  ----  " << radious << " " << radiousBullet << endl;
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



	// Función para actualizar el valor de la rotación del enemigo enemyIndex
    void actualizarRotacion(int enemyIndex, float deltaTime) {
        // Si está apuntando el ángulo es similar al objetivo, no hacer nada
        if (states[enemyIndex] == APUNTANDO) {
            float angle = currentRotation[enemyIndex] - goalRotation[enemyIndex];
            if (angle < 0) angle += 360.0f; // Hacemos el módulo
            if (angle < rotationSpeed * deltaTime || angle > 360.0f - rotationSpeed * deltaTime) return;
        }
        cout << "Current rotation: " << currentRotation[enemyIndex] << ", goalRotation: " << goalRotation[enemyIndex] << endl;
        float diff = currentRotation[enemyIndex] - goalRotation[enemyIndex];
		if (diff>180.0f || (-180<diff && diff<0)) {
			currentRotation[enemyIndex] += rotationSpeed * deltaTime;
            float aux = currentRotation[enemyIndex];
            if (currentRotation[enemyIndex] > 360.0f) currentRotation[enemyIndex] -= 360.0f;
            cout << "girando antihorario" << endl;
            cout << "next rotation: " << currentRotation[enemyIndex] << ", goalRotation: " << goalRotation[enemyIndex] << endl;
            bool andar = false;

            if (360.0f - currentRotation[enemyIndex] < rotationSpeed * deltaTime) currentRotation[enemyIndex] = 0.0f;
            
            if (aux < goalRotation[enemyIndex]) andar = currentRotation[enemyIndex] >= goalRotation[enemyIndex];
            else andar = currentRotation[enemyIndex] < 10;

            // Si la rotación actual es cercana a 360º, se pone a 0
            if ( andar && states[enemyIndex] == GIRANDO) {
                //cout << prevIndex[enemyIndex].x << " " << prevIndex[enemyIndex].y << "false" << endl;
                map[prevIndex[enemyIndex].x][prevIndex[enemyIndex].y] = false;
                states[enemyIndex] = ANDANDO;
            }
		}
		else{
            cout << "girando horario" << endl;
			currentRotation[enemyIndex] -= rotationSpeed * deltaTime;
            
            if (currentRotation[enemyIndex] < 0.0f) currentRotation[enemyIndex] += 360.0f;
            if (currentRotation[enemyIndex] <= goalRotation[enemyIndex] && states[enemyIndex] == GIRANDO) {
                //cout << prevIndex[enemyIndex].x << " " << prevIndex[enemyIndex].y << "false" << endl;
                map[prevIndex[enemyIndex].x][prevIndex[enemyIndex].y] = false;
                states[enemyIndex] = ANDANDO;
            }
            //cout << "next rotation: " << currentRotation[enemyIndex] << ", goalRotation: " << goalRotation[enemyIndex] << endl;
		}
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
        pintarEnemigo(enemyIndex, shader); // Simplemente lo pintamos
    }


    bool pararAndar(int enemyIndex) {
        float x = roundf((positions[enemyIndex].x + directions[enemyIndex].x)/* * deltaTime */ * 100) / 100;
        float z = roundf((positions[enemyIndex].z + directions[enemyIndex].z)/* * deltaTime */ * 100) / 100;
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
    void gestionarAndando(int enemyIndex, Shader& shader, float deltaTime) {
		//Calculamos las actualizaciones necesarias
        float start = map.size() * dim / 2;
        //if ((positions[enemyIndex].x == destiny[enemyIndex].x) && (positions[enemyIndex].z == destiny[enemyIndex].z)) {
        if(pararAndar(enemyIndex)){
            positions[enemyIndex] = destiny[enemyIndex];
            glm::vec3 prevDir = directions[enemyIndex];
            prevIndex[enemyIndex] = index[enemyIndex];
            index[enemyIndex] = nextIndex(static_cast<int>(index[enemyIndex].x), static_cast<int>(index[enemyIndex].y), directions[enemyIndex]);
            if (prevDir != directions[enemyIndex]) {
                cout << "Cambiando dir" << prevDir.x<< " " << prevDir.z << " --> " << directions[enemyIndex].x << " " << directions[enemyIndex].z << endl;
                updateGoalRotation(enemyIndex);
                states[enemyIndex] = GIRANDO;
            }
            else {
                map[prevIndex[enemyIndex].x][prevIndex[enemyIndex].y] = false;
            }
            destiny[enemyIndex] = glm::vec3(-start + index[enemyIndex].y * dim, 0, start - index[enemyIndex].x * dim);
        }
        if (states[enemyIndex] == ANDANDO) { // Si seguimos moviéndonos
			//cout << "directions: " << directions[enemyIndex].x << " " << directions[enemyIndex].y << " " << directions[enemyIndex].z << endl;
            positions[enemyIndex].x = roundf((positions[enemyIndex].x + directions[enemyIndex].x)/* * deltaTime */ * 100) / 100;
            positions[enemyIndex].z = roundf((positions[enemyIndex].z + directions[enemyIndex].z)/* * deltaTime */ * 100) / 100;
            //cout << "positions = " << positions[enemyIndex].x << ", " << positions[enemyIndex].z << endl;
        }
        
        // Y al final dibujamos las caras :)
        pintarEnemigo(enemyIndex, shader);
    }

	// Gestionar el movimiento cuando el enemigo está girando
	void gestionarGirando(int enemyIndex, Shader& shader, float deltaTime) {
        actualizarRotacion(enemyIndex, deltaTime); // Rotar lo que sea necesario
        pintarEnemigo(enemyIndex, shader); // Pintar el enemigo como tal
		
	}

    int cont = 0;
    // Gestionar el movimiento cuando el enemigo está girando
    void gestionarApuntando(int enemyIndex, Shader& shader, glm::vec3 playerPosition, float deltaTime) {
        // Vector que va desde el enemigo al jugador
		glm::vec3 enemyToPlayerVec = playerPosition - positions[enemyIndex]; 
        float deg = -atan(enemyToPlayerVec[0] / enemyToPlayerVec[2]) * 180.0f / 3.1415f;
        //if (deg < 0.0) deg = 180.0f - deg;
        if (playerPosition.z < positions[enemyIndex].z) {
			deg = 180.0f + deg;
        }
		if (deg < 0.0) deg = 360.0f + deg;
        
        cont++;
        if (cont == 200) {
			cout << "Jugador: " << playerPosition.x << ", " << playerPosition.z << 
                ". Enemigo: " << positions[enemyIndex].x << ", " << positions[enemyIndex].z << ". Angulo: " << deg << endl;
            cont = 0;
        }
		
        goalRotation[enemyIndex] = deg;
        actualizarRotacion(enemyIndex, deltaTime); // Rotar lo que sea necesario
        pintarEnemigo(enemyIndex, shader); // Pintar el enemigo como tal

    }
	
    void DrawEnemies(Shader& shader, glm::vec3 playerPosition, float deltaTime) {
        hit_time = static_cast<int>(30000 * deltaTime);
        for (int i = 0; i < numEnemies; i++) {
            if (vidas[i] > 0) {
				//cout << "Enemigo " << i << ": " << states[i] << endl;
                switch (states[i]) {
                case PARADO: {gestionarParado(i, shader); break;}
                case ANDANDO: {gestionarAndando(i, shader, deltaTime); break;}
                case GIRANDO: {gestionarGirando(i, shader, deltaTime); break;}
                case APUNTANDO: {gestionarApuntando(i, shader, playerPosition, deltaTime); break;}
                default: {cerr << "Error en el estado del enemigo " << i << endl; break;}
                }
                
            }
        }
    };

};