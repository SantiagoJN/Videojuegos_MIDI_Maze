#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Bullets.h>
#include <map.h>
#include <wall.h>
#include <Enemies.h>

#include <Camera.h>
#include <model.h>

#include <iostream>

#include <assimp/scene.h>

#include <glut/glut.h>

#include <irrKlang/irrKlang.h>
using namespace irrklang;

bool newBullet = false;

//ISoundEngine* SoundEngine = createIrrKlangDevice(); // to manage the sound effects

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void showFPS(int fps);

// Initial settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f; // Initial values in the middle of the screen
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


// valores para normalizar el movimiento de la c�mara
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float lastFrameFPS = 0.0f;
int countFrames = 0; // Para saber los frames que ha habido en 1s

int main()
{
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

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    //Model tests;

    // =====================================================================================================================
    // ================================== CONFIGURATION OF SHADERS, SHAPES AND TEXTURES ====================================
    // =====================================================================================================================

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("shaders/1.model_loading.vs", "shaders/1.model_loading.fs");


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);    //Capturar el rat�n
    glfwSetCursorPosCallback(window, mouse_callback);

    Bullet myBullets("resources/objects/sphere.obj", 0.1);

    Map pared("resources/maps/map1.txt", ourShader);
    //Wall pared(glm::vec3(0, 0, 0), glm::vec3(5, 0, 0), ourShader);


    Enemy myEnemies("resources/objects/sphere.obj", 0.5, 40, pared.getLab(), pared.getDim());
    // =====================================================================================================================
    // ==================================================== RENDER LOOP ====================================================
    // =====================================================================================================================
    while (!glfwWindowShouldClose(window))
    {
        //tests = mapa;

        // Datos para gestionar los fps
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
		
        countFrames++; // Actualizar los frames en el �ltimo segundo
		// Datos para gestionar los fps
        /*if (currentFrame - lastFrameFPS > 1.0f) {
            cout << "FPS: " << countFrames << endl;
            showFPS(countFrames);
			countFrames = 0;
			lastFrameFPS = currentFrame; // Actualizamos
        }*/

        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!


        // activate shader
        ourShader.use();

        pared.Draw(ourShader);

        // create transformations
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.

        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        if (newBullet) {
            newBullet = false;
            myBullets.newBullet(camera.Position, camera.Front);
        }

        myBullets.DrawBullets(ourShader,myEnemies);
        
        myEnemies.DrawEnemies(ourShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


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
    glViewport(0, 0, width, height);
}



void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
/*void processInput(GLFWwindow* window)                                                                                     MOVEMENT JESUS
{   
    bool moveF = true;
    bool moveB = true;
    bool moveL = true;
    bool moveR = true;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
       
        moveF = tests.checkCollisionsModel(camera.Position + (camera.Front * (camera.MovementSpeed * deltaTime)));
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime, moveF, moveB, moveL, moveR);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        moveB = tests.checkCollisionsModel(camera.Position - (camera.Front * (camera.MovementSpeed * deltaTime)));
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime, moveF, moveB, moveL, moveR);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		moveL = tests.checkCollisionsModel(camera.Position - (camera.Right * (camera.MovementSpeed * deltaTime)));
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime, moveF, moveB, moveL, moveR);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        moveR = tests.checkCollisionsModel(camera.Position + (camera.Right * (camera.MovementSpeed * deltaTime)));
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime, moveF, moveB, moveL, moveR);
    }
}*/

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) { // initially set to true
        lastX = static_cast<float>(xpos); // To avoid warning messages
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }


    float xoffset = static_cast<float>(xpos - lastX);
    float yoffset = static_cast<float>(lastY - ypos); // reversed since y-coordinates range from bottom to top
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        //cout << camera.Position[0] << "," << camera.Position[1] << "," << camera.Position[2] << endl;
        //cout << camera.Front[0] << "," << camera.Front[1] << "," << camera.Front[2] << endl;

        newBullet = true;
        SoundEngine->play2D("resources/effects/disparo.mp3", false); //Play the sound without loop
    }
}

void showFPS(int fps) {
    /*char string[5];
	sprintf_s(string, "%d", fps);
    //unsigned char string[] = to_string(fps);
    int w = glutBitmapLength(GLUT_BITMAP_8_BY_13, reinterpret_cast<unsigned char*>(string));
    glRasterPos2f(0., 0.);
    float x = .5;
    glRasterPos2f(x - (float)SCR_WIDTH / 2, 0.);
    //glColor(1., 0., 0.);
    int len = strlen(string);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, string[i]);
    }*/
}