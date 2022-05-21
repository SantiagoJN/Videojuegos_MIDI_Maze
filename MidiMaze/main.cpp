#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <Windows.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Bullets.h>
#include <EnemyBullets.h>
#include <map.h>
#include <wall.h>
#include <Enemies.h>

#include <Camera.h>
#include <model.h>
#include <scr_Princ.h>
#include <leaveGame.h>
#include <statusIngame.h>
#include <mirilla.h>
#include <kills.h>
#include <showScreen.h>
#include <menuInGame.h>

#include <iostream>

#include <assimp/scene.h>

#include <glut/glut.h>

#include <irrKlang/irrKlang.h>
using namespace irrklang;

bool WON, regenerando;
bool processGame;

double prevXM, prevYM;

# define M_PI           3.14159265358979323846  /* pi */

bool pressed = false;
bool newBullet = false;

bool first1 = true;

enum volume_types { MUTE, BAJO, NORMAL };

//ISoundEngine* SoundEngine = createIrrKlangDevice(); // to manage the sound effects

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void menu_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void nada(GLFWwindow* window, double xpos, double ypos);
void initMenuInGame();


// Initial settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float screenMinX = 0;
float screenMinY = 0;
float screenMaxX = SCR_WIDTH;
float screenMaxY = SCR_HEIGHT;
float screenMaxRelativeX = SCR_WIDTH;
float screenMaxRelativeY = SCR_HEIGHT;									 

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f; // Initial values in the middle of the screen
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//HUD in game
menuTop topHUD;
menuBottom botHUD;
menuRight rightHUD;
menuLeft leftHUD;


double lastButtonX = 0.0f;
double lastButtonY = 0.0f;

Shader ourShader;

// valores para normalizar el movimiento de la c�mara
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame


float lastFrameFPS = 0.0f;
int countFrames = 0; // Para saber los frames que ha habido en 1s

unsigned int currentDelay = 0; // Delay inicializado a 0. Cuando se dispare, se pondrá un contador u otro dependiendo de la cadencia
int vidas = 3;

gameLeaver leave;

Map temp;


bool initGame = false;

// ###Constantes varias###
bool versionModerna = true;
bool versionModernaGraficos = false;
bool relSpeed;
float volume = 0.1f;
int currentRegenTime;
bool oPresionado = false;
bool lPresionado = false;

bool tocaActualizar = false;

int main()
{
    //::ShowWindow(::GetConsoleWindow(), SW_SHOW);
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    // =====================================================================================================================
    // =================================== INITIALIZATION AND CONFIGURATION OF LIBRARIES ===================================
    // =====================================================================================================================
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    

    // If in an Apple environment, declare this thing below
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Midi Maze :)", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))                //Initial
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);


    // build and compile our shader zprogram
    // ------------------------------------
    ourShader = Shader("shaders/1.model_loading.vs", "shaders/1.model_loading.fs");
    SoundEngine->setSoundVolume(volume);
    /*
    glm::vec3 v1(-5, -3, -4.2);
    glm::vec3 v2(5, -3, -4.2);

    double totalx = v2.x - v1.x;
    double totaly = 3 - v1.y;
    Despleg desp(glm::vec3(v1.x + 0.175 * totalx, v1.y + 0.7 * totaly, v1.z), glm::vec3(v1.x + 0.46 * totalx, v1.y + 0.7 * totaly, v1.z), ourShader);
    */
    

    //gameLeaver leave(glm::vec3(-0.04, -0.01, 2.88), glm::vec3(0.04, -0.01, 2.88),camera.getPosition(), ourShader);

    cout << camera.getPosition().x << "," << camera.getPosition().y << "," << camera.getPosition().z << endl;
    cout << camera.Front.x << "," << camera.Front.y << "," << camera.Front.z << endl;


    showScreen win = showScreen(camera.getPosition(), camera.Front, ourShader, "resources/Fotos_midi_maze/winner/", "player");
    showScreen dead = showScreen(camera.getPosition(), camera.Front, ourShader, "resources/Fotos_midi_maze/dead/", "blue");
    leave = gameLeaver(camera.getPosition(), camera.Front, ourShader);
    //leave = gameLeaver(glm::vec3(camera.getPosition().x -0.4, -0.01, camera.getPosition().z - 0.12), glm::vec3(camera.getPosition().x+0.4, -0.01, camera.getPosition().z - 0.12), camera.getPosition(), ourShader);
    Princip menu;
	initMenuInGame();
    totalPoints pointsGame;
	pointsGame = totalPoints(camera.getPosition(), camera.Front, ourShader);
    jugadorMuertos muertesJugador;
	
    while (!glfwWindowShouldClose(window)) {
        menu = Princip(glm::vec3(-4, -3, -4.2), glm::vec3(4, -3, -4.2), ourShader);
        camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
        glfwSetMouseButtonCallback(window, menu_mouse_button_callback);
        leave = gameLeaver(camera.getPosition(), camera.Front, ourShader);
        processGame = false;
        bool finish = false;
        initGame = false;
        while (!glfwWindowShouldClose(window) && !finish)
        {
            // input
            if (pressed) {
                pressed = !pressed;
                finish = menu.checkButton(lastButtonX, lastButtonY, ourShader, window);
                int set_volume = menu.config.vol.getVolume();           //VOLUME 0(mute) 1(bajo) 2(normal)
                //cout << "Set_volume = " << set_volume << endl;
                switch (set_volume) {
                    case MUTE: {
                        volume = 0.0f;
                        //cout << "volume: mute" << endl;
                        break;
                    }
                    case BAJO: {
                        volume = 0.1f;
                        //cout << "volume: bajo" << endl;
                        break;
                    }				
                    case NORMAL: {
                        volume = 0.6f;
                        //cout << "volume: normal" << endl;
                        break;
                    }
                    default: {
                        //cerr << "Something went wrong with the volumes :S" << endl;
                    }
                }
                //cout << "volumen actual: " << volume << endl;
                SoundEngine->setSoundVolume(volume);
            }

            // render
            //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClearColor(0,0,0, 1.0f); // Los colores del juego
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

            // create transformations
            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.

            // activate shader
            ourShader.use();

            menu.draw(ourShader);
            processInput(window);

            glm::mat4 view = camera.GetViewMatrix();

            ourShader.setMat4("view", view);



            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();

        }
        if (glfwWindowShouldClose(window)) break;
        menu.desplegable.start.buttonCalled();
        menu.desplegable.settings.buttonCalled();
        // =====================================================================================================================
        // ======================================================== GAME =======================================================
        // =====================================================================================================================

        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);    //Capturar el rat�n
        glfwSetCursorPosCallback(window, mouse_callback);



        versionModerna = !menu.config.getMovimientoOriginal();      //VERSION MODERNA RATÓN 
        
        bool IAModerna = !menu.config.getIAOriginal();     //Versión moderna IA

        versionModernaGraficos = !menu.config.appearence.getOriginal();

        bool safeRegen = !menu.config.safer.getOriginal();

        Bullet myBullets(0.1);
        vector<EnemBullet> enemyBullets;

        mirilla mira("resources/objects/bullets/yellow/yellow.obj");

        cout << "Jugando con mapa " << menu.getMapName() << endl;
        Map pared(menu.getMapName(), ourShader);
        vidas = menu.getNumVidas();

        bool regenSpeed = menu.getRegenSpeed();
        bool reviveSpeed = menu.getReviveSpeed();           //Velocidades
        bool reloadSpeed = menu.getReloadSpeed();
        relSpeed = reloadSpeed;
		killsPlayer kills;
        if (versionModernaGraficos) {
            kills = killsPlayer(camera.getPosition(), camera.Front, ourShader);
        }
        statusPlayer status(glm::vec3(-4, -3, -4.2), glm::vec3(4, -3, -4.2),camera.getPosition(), camera.Front, ourShader, vidas,versionModernaGraficos);
        Enemy myEnemies;
        if (!glfwWindowShouldClose(window)) {
            myEnemies = Enemy(0.5, vidas, menu.getVeryDumb(), menu.getPlainDumb(), menu.getNotDumb(), pared.getLab(), pared, 
                pared.getDim(), menu.getRegenSpeed(), menu.getReloadSpeed(), menu.getReviveSpeed(), IAModerna);
        }
        cout << "Fuera enemigos" << endl;
        int puntJug = 0;
        WON = false;
        regenerando = false;
        processGame = true;

        initGame = true;
        // =====================================================================================================================
        // ==================================================== GAME LOOP ====================================================
        // =====================================================================================================================
		
		
        if (!versionModernaGraficos) {
            muertesJugador = jugadorMuertos(camera.getPosition(), camera.Front, ourShader, myEnemies);
        }

        float tamX = 0;
        float tamY = 0;
        float iniX = 0;
        float iniY = 0;
        newBullet = false;  //Bug de que empieza disparando
        temp = pared;
        while (!glfwWindowShouldClose(window))
        {
			if (versionModernaGraficos) {
                tamX = screenMaxRelativeX;
                tamY = screenMaxRelativeY;
                iniX = screenMinX;
                iniY = screenMinY;
            }
            else {
                tamX = screenMaxRelativeX * 0.4895;
                tamY = screenMaxRelativeY * 0.479;
                iniX = screenMinX + (tamX * 0.115f);
                iniY = screenMinY + (tamY * 0.607f);
            }

            glViewport(iniX, iniY, tamX, tamY);											 

            if (pressed) {
                pressed = !pressed;
				glViewport(screenMinX, screenMinY, screenMaxRelativeX, screenMaxRelativeY);																	   
                int button = leave.checkButton(lastButtonX, lastButtonY, ourShader);
                if (button == 1) {
                    glfwSetMouseButtonCallback(window, mouse_button_callback);
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);    //Capturar el rat�n
                    glfwSetCursorPosCallback(window, mouse_callback);
                    leave.show();
                }
                else if (button == 0) break;
            }
			glViewport(iniX, iniY, tamX, tamY);
            

            // Datos para gestionar los fps
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            countFrames++; // Actualizar los frames en el �ltimo segundo
            // Datos para gestionar los fps
            if (currentFrame - lastFrameFPS > 1.0f) {
                cout << "FPS: " << countFrames << endl;
                countFrames = 0;
                lastFrameFPS = currentFrame; // Actualizamos
            }
            if (currentDelay > 0) currentDelay--; // Decrementar el contador de delay para el disparo del jugador

            
            // input
            if (!leave.pause()) processInput(window);

            // render
            //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            //glClearColor(0.239f, 0.298f, 0.917f, 1.0f); // Los colores del juego
            glClearColor(0, 0, 0, 1.0f); // Los colores del juego
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

			glViewport(iniX, iniY, tamX, tamY);
            if (currentRegenTime > 0) {
                myEnemies.blinded();
            }
            // activate shader
            ourShader.use();

			glViewport(iniX, iniY, tamX, tamY);
            pared.Draw(ourShader);
            // create transformations
            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.

            glm::mat4 view = camera.GetViewMatrix();
            ourShader.setMat4("view", view);

			glViewport(iniX, iniY, tamX, tamY);
            if (newBullet) {
                newBullet = false;
                myBullets.newBullet(glm::vec3(camera.Position+camera.Front), camera.Front);
            }

			glViewport(iniX, iniY, tamX, tamY);
            //cout << camera.Position[0] << ", " << camera.Position[1] << ", " << camera.Position[2] << ", " << endl;
            bool hayHit = false;
            myBullets.DrawBullets(ourShader, myEnemies, pared, deltaTime, leave.pause() || WON, camera.Position, hayHit);
            if (hayHit) {
                tocaActualizar = true;
                //cout << "Ha habido un hit" << endl;
            }
            int balasRecibidas = 0;
            bool ganaEnemigo = false;
            bool mataEnemigo = false;
            string nombreGanador = "";
            int lastVidas = vidas;
            
            bool justRegen = currentRegenTime == 1 && safeRegen; // Si justo reaparece, y está activa la opción
            if (justRegen) myEnemies.setInv(deltaTime);
			
			glViewport(iniX, iniY, tamX, tamY);
            myEnemies.DrawEnemies(ourShader, camera.Position, myEnemies, pared, deltaTime, balasRecibidas, vidas, 
                leave.pause() || WON || regenerando, ganaEnemigo, mataEnemigo, nombreGanador);
            if (lastVidas != vidas) status.setUp(ourShader, vidas);
            if (mataEnemigo) cout << "--Te ha matado el bot " << nombreGanador << endl;
            if (ganaEnemigo) {
                win.setUp(ourShader, nombreGanador);
                WON = true;
                cout << "******Ha ganado el bot " << nombreGanador << "*******" << endl;
            }
            if (vidas <= 0) {
                glm::vec2 pos = myEnemies.getFreePosition();
                camera.updatePosition(glm::vec3(pos.x, 0.0, pos.y));
                vidas = menu.getNumVidas();
                //status.setUp( ourShader, vidas);
                if (reviveSpeed) currentRegenTime = static_cast<int>(spawnTime[SPAWN_RAPIDO] / deltaTime);
                else currentRegenTime = static_cast<int>(spawnTime[SPAWN_LENTO] / deltaTime);
                status.setUp(ourShader, 0);
                dead.setUp(ourShader, nombreGanador);
            }

            if (balasRecibidas > 0) {
                status.setUp(ourShader, vidas);
                tocaActualizar = true;
                //cout << "He recibido bala D:" << endl;
            }


            if (currentRegenTime > 1) {
                regenerando = true;
                myEnemies.blinded();
                //cout << "Regenerando... " << currentRegenTime << endl;
                currentRegenTime--;
				if (!versionModernaGraficos) {
                    if (first1) {
                        status.setUp(ourShader, 0);
                        first1 = false;
                    }
                    status.draw(camera.getPosition(), camera.Front, camera.Pitch, ourShader, screenMinX, screenMaxRelativeX, screenMinY, screenMaxRelativeY);
                }
				glViewport(iniX, iniY, tamX, tamY);
                dead.draw(camera.getPosition(), camera.Front, camera.Pitch, ourShader);
            }
            else if (currentRegenTime == 1) {
                status.setUp(ourShader, vidas);
                currentRegenTime--;
            }
            else{
                if (!WON && versionModernaGraficos) kills.draw(camera.getPosition(), camera.Front, camera.Pitch, ourShader);
                if (!WON) status.draw(camera.getPosition(), camera.Front, camera.Pitch, ourShader,screenMinX, screenMaxRelativeX,screenMinY,screenMaxRelativeY);
                
                regenerando = false;
                first1 = true;
            }

			glViewport(screenMinX, screenMinY, screenMaxRelativeX, screenMaxRelativeY);
            leave.draw(ourShader);
			glViewport(iniX, iniY, tamX, tamY);	
			
            if (myEnemies.getPuntuacionJugador() != puntJug) {
                puntJug = myEnemies.getPuntuacionJugador();
                if (versionModernaGraficos) {
                    kills.setUp(ourShader, puntJug);
                }
                
            }

            if (WON) {
				glViewport(iniX, iniY, tamX, tamY);																	   
                win.draw(camera.getPosition(), camera.Front, camera.Pitch, ourShader);
            }
            else if (puntJug > 9) {
				glViewport(iniX, iniY, tamX, tamY);																	   
                cout << "Winner" << endl;
                WON = true;
                win.draw(camera.getPosition(), camera.Front, camera.Pitch, ourShader);
            }

			glViewport(screenMinX, screenMinY, screenMaxRelativeX, screenMaxRelativeY);

            //glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.

            // activate shader
            ourShader.use();
            
            if (tocaActualizar == true && !versionModernaGraficos) {
                muertesJugador.actualizar(camera.getPosition(), camera.Front, ourShader, myEnemies);
                tocaActualizar = false;
            }
			
			if (!versionModernaGraficos) {
                topHUD.drawInGame(camera, ourShader);
                botHUD.drawInGame(camera, ourShader);
                leftHUD.drawInGame(camera, ourShader);
                rightHUD.drawInGame(camera, ourShader);
                pointsGame.draw(camera.getPosition(), camera.Front, camera.Pitch, ourShader, screenMinX, screenMaxRelativeX, screenMinY, screenMaxRelativeY, myEnemies);
                muertesJugador.draw(camera.getPosition(), camera.Front, camera.Pitch, ourShader, screenMinX, screenMaxRelativeX, screenMinY, screenMaxRelativeY, myEnemies);
            }
			

            if(!leave.pause() && !WON && currentRegenTime <= 0) mira.draw(camera.getPosition(), camera.Front, ourShader, iniX, tamX, iniY ,tamY);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();

        }
        currentRegenTime = 0;
    }
	glViewport(screenMinX, screenMinY, screenMaxRelativeX, screenMaxRelativeY);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// =====================================================================================================================
// ================================================ CONFIGURE CALLBACKS ================================================
// =====================================================================================================================

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    //cout << "width: " << width << " height: " << height << endl;

    // Calculate the proper aspect ratio to use based on window ratio
    float ratioX = width / (float)SCR_WIDTH;
    float ratioY = height / (float)SCR_HEIGHT;
    float ratio = ratioX < ratioY ? ratioX : ratioY;
    // Calculate the width and height that the will be rendered to
    float viewWidth = (SCR_WIDTH * ratio);
    float viewHeight = (SCR_HEIGHT * ratio);
    // Calculate the position, which will apply proper "pillar" or "letterbox" 
    float viewX = (width - SCR_WIDTH * ratio) / 2;
    float viewY = (height - SCR_HEIGHT * ratio) / 2;
    // Apply the viewport and switch back to the GL_MODELVIEW matrix mode
    glViewport(viewX, viewY, viewWidth, viewHeight);

    screenMinX = viewX;
    screenMinY = viewY;
    screenMaxX = viewX + viewWidth;
    screenMaxY = viewY + viewHeight;
	screenMaxRelativeX = viewWidth;
    screenMaxRelativeY = viewHeight;							   

    cout << "menor x: " << viewX << "menor y: " << viewY << "mayorX: " << viewX + viewWidth << "mayorY: " << viewY + viewHeight << endl;
}

void nada(GLFWwindow* window, double xpos, double ypos) {}

void processInput(GLFWwindow* window)
{
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    //REPASARLO
    if (processGame) {
        float velocity = camera.MovementSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glViewport(screenMinX, screenMinY, screenMaxRelativeX, screenMaxRelativeY);
            leave = gameLeaver(camera.getPosition(), camera.Front, ourShader);
            leave.show();
            glfwSetMouseButtonCallback(window, menu_mouse_button_callback);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);    //Capturar el rat�n
            glfwSetCursorPosCallback(window, nada);
            //glfwSetWindowShouldClose(window, true);
        }
        else if (!WON && !regenerando) {
            if (versionModerna) {
                // ############ VERSIÓN MODERNA ############
                bool slow = false;
                if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) ||
                    (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) ||
                    (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) ||
                    (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)) {
                    //cout << "Hey " << velocity<<endl;
                    velocity = velocity / 2;
                    //cout << "Slowed " << velocity << endl;
                    slow = true;
                }
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                    up = temp.checkIntersections(camera.Position, (camera.Position + (camera.Front * velocity)));
                    camera.ProcessKeyboard(Camera_Movement::FORWARD, velocity, up, down, left, right);
                }

                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                    down = temp.checkIntersections(camera.Position, (camera.Position - (camera.Front * velocity)));
                    camera.ProcessKeyboard(Camera_Movement::BACKWARD, velocity, up, down, left, right);
                }

                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                    left = temp.checkIntersections(camera.Position, (camera.Position - (camera.Right * velocity)));
                    camera.ProcessKeyboard(Camera_Movement::LEFT, velocity, up, down, left, right);
                }

                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                    right = temp.checkIntersections(camera.Position, (camera.Position + (camera.Right * velocity)));
                    camera.ProcessKeyboard(Camera_Movement::RIGHT, velocity, up, down, left, right);
                }
                if (slow) {
                    velocity = velocity * 2;
                    //cout << "ou " << velocity << endl;
                }
            }
            // ############ VERSIÓN VIEJA ############
            else {
                if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
                    up = temp.checkIntersections(camera.Position, (camera.Position + (camera.Front * velocity)));
                    camera.ProcessKeyboard(Camera_Movement::FORWARD, velocity, up, down, left, right);
                }   
                if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                    down = temp.checkIntersections(camera.Position, (camera.Position - (camera.Front * velocity)));
                    camera.ProcessKeyboard(Camera_Movement::BACKWARD, velocity, up, down, left, right);
                }
                if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
                    if (currentDelay == 0 && currentRegenTime <= 0) { // Puedo disparar
                        newBullet = true;
                        SoundEngine->play2D("resources/effects/disparo.mp3", false); //Play the sound without loop
                        if (relSpeed) currentDelay = static_cast<unsigned int>(reloadTime[CAD_RAPIDA] / deltaTime);
                        else currentDelay = static_cast<unsigned int>(reloadTime[CAD_LENTA] / deltaTime);
                    }
                }
                if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                    float xoffset = 1300.0f * deltaTime;
                    float yoffset = 0.0;
                    camera.ProcessMouseMovement(xoffset, yoffset);
                }
                if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                    float xoffset = -1300.0f * deltaTime;
                    float yoffset = 0.0;
                    camera.ProcessMouseMovement(xoffset, yoffset);
                }

            }
        }
    }
    //ACCIONES DURANTE TODO EL BUCLE
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && !oPresionado) {
        oPresionado = true;
        if (volume < 1.0) volume += 0.1f;
        SoundEngine->setSoundVolume(volume);
        //cout << "Volumen subido a " << volume << endl;
        SoundEngine->play2D("resources/effects/plik.mp3", false);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !lPresionado) {
        lPresionado = true;
        if (volume > 0.0) volume -= 0.1f;
        SoundEngine->setSoundVolume(volume);
        //cout << "Volumen bajado a " << volume << endl;
        SoundEngine->play2D("resources/effects/plik.mp3", false);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_RELEASE) oPresionado = false;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) lPresionado = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) { // initially set to true
        lastX = static_cast<float>(xpos); // To avoid warning messages
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }
    if (!WON && !regenerando) {
        if (versionModerna) {
            float xoffset = static_cast<float>(xpos - lastX);
            float yoffset = static_cast<float>(lastY - ypos); // reversed since y-coordinates range from bottom to top
            lastX = static_cast<float>(xpos);
            lastY = static_cast<float>(ypos);
            //leave = gameLeaver(glm::vec3(camera.getPosition().x - 0.04, -0.01, camera.getPosition().z - 0.12), glm::vec3(camera.getPosition().x + 0.04, -0.01, camera.getPosition().z - 0.12), camera.getPosition(), ourShader);
            camera.ProcessMouseMovement(xoffset, yoffset);
        }
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (versionModerna) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            //cout << currentDelay<<" "<<deltaTime<<" "<< reloadTime[CAD_RAPIDA] << endl;
            if (!WON) {
                if (currentDelay == 0 && currentRegenTime <=0) { // Puedo disparar
                    //leave = gameLeaver(glm::vec3(camera.getPosition().x - 0.04, -0.01,camera.Front.z * (camera.getPosition().z - 0.12)), glm::vec3(camera.getPosition().x + 0.04, -0.01, camera.Front.z * (camera.getPosition().z - 0.12)), camera.getPosition(), ourShader);
                    newBullet = true;
                    SoundEngine->play2D("resources/effects/disparo.mp3", false); //Play the sound without loop
                    //currentDelay = static_cast<unsigned int>(reloadTime[CAD_RAPIDA] / deltaTime);
                    if (relSpeed) currentDelay = static_cast<unsigned int>(reloadTime[CAD_RAPIDA] / deltaTime);
                    else currentDelay = static_cast<unsigned int>(reloadTime[CAD_LENTA] / deltaTime);

                }
            }
            //cout << camera.Position[0] << "," << camera.Position[1] << "," << camera.Position[2] << endl;
            //cout << camera.Front[0] << "," << camera.Front[1] << "," << camera.Front[2] << endl;

            
        }
    }
}

void menu_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        cout << xpos << "," << ypos << endl;
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        cout << width << "," << height << endl;
        cout <<"Total\t" << xpos / width << "," << ypos / height << endl;
        if (xpos >= screenMinX && xpos <= screenMaxX && ypos >= screenMinY && ypos <= screenMaxY) {
            lastButtonX = (xpos - screenMinX) / (screenMaxX - screenMinX);
            lastButtonY = (ypos - screenMinY) / (screenMaxY - screenMinY);
            if (!initGame) pressed = true;
            else if (leave.shown) pressed = true;
            cout << "Parcial\t"<<(xpos-screenMinX)/(screenMaxX-screenMinX)<<","<< (ypos - screenMinY) / (screenMaxY - screenMinY)<<endl;
        }
        //lastButtonX = xpos / width;
        //lastButtonY = ypos / height;
        
    }
}

void initMenuInGame() {
    topHUD = menuTop(glm::vec3(-4, -3, -4.2), glm::vec3(4, -3, -4.2), ourShader);
    botHUD = menuBottom(glm::vec3(-4, -3, -4.2), glm::vec3(4, -3, -4.2), ourShader);
    rightHUD = menuRight(glm::vec3(-4, -3, -4.2), glm::vec3(4, -3, -4.2), ourShader);
    leftHUD = menuLeft(glm::vec3(-4, -3, -4.2), glm::vec3(4, -3, -4.2), ourShader);
}

bool primeravez = true;
