#pragma once
#include <glm/ext/vector_float3.hpp>
#include <model.h>
#include <Enemies.h>

/*string basePath = "resources/objects/smileys/";
string colors[] = { "blue", "orange", "green", "pink", "lightblue", "magenta", "white",
                   "yellow2", "blue2", "orange2", "green2", "pink2", "lightblue2", "magenta2", "white2" };*/


class menuTop {

public: 

	glm::vec3 position = glm::vec3(0, 0, 0);

    menuTop(glm::vec3 v1, glm::vec3 v2, Shader& ourShader) {
        vertices[0] = v1.x;
        vertices[1] = v1.y;
        vertices[2] = v1.z;

        vertices[3] = 0.0f;
        vertices[4] = 0.0f;

        vertices[5] = v2.x;
        vertices[6] = v2.y;
        vertices[7] = v2.z;

        vertices[8] = 1.0f;
        vertices[9] = 0.0f;

        vertices[10] = v1.x;
        vertices[11] = v1.y + 1.45;
        vertices[12] = v1.z;

        vertices[13] = 0.0f;
        vertices[14] = 1.0f;

        vertices[15] = v2.x;
        vertices[16] = v2.y + 1.45;
        vertices[17] = v2.z;

        vertices[18] = 1.0f;
        vertices[19] = 1.0f;

        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 1;
        indices[4] = 2;
        indices[5] = 3;

        setUpWall(ourShader);

        if (v1.x != v2.x) normal = glm::vec2(0, 1);
        else normal = glm::vec2(1, 0);
    };

    menuTop() {};

    void draw(Shader& ourShader) {

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 4.6f, 0)); // translate it so it's at the center of the scene
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };

    void drawInGame(Camera camera, Shader& ourShader) {

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glm::mat4 viewMatrixInverse = glm::inverse(camera.GetViewMatrix());
        glm::vec3 cameraPositionWorldSpace = glm::vec3(viewMatrixInverse[3][0], viewMatrixInverse[3][1], viewMatrixInverse[3][2]);


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, (cameraPositionWorldSpace + camera.Front * 3.0f) + glm::vec3(0, 4.6f, 0)); // translate it so it's at the center of the scene

        float angle = atan(camera.Front.x / camera.Front.z) * 180.0f / 3.1415f;
        if (camera.Front.z > 0) angle -= 180;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };

private:
    // model data 
    float vertices[20];
    glm::vec2 normal;
    unsigned int indices[6];
    unsigned int VBO, VAO, EBO;
    unsigned int texture1;

    void setUpWall(Shader& ourShader) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindVertexArray(0);


        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        unsigned char* data;
            data = stbi_load("resources/Fotos_midi_maze/menuTop.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        ourShader.use();
        glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);

    }




};

class menuBottom {

public:

    glm::vec3 position = glm::vec3(0, 0, 0);

    menuBottom(glm::vec3 v1, glm::vec3 v2, Shader& ourShader) {
        vertices[0] = v1.x;
        vertices[1] = v1.y;
        vertices[2] = v1.z;

        vertices[3] = 0.0f;
        vertices[4] = 0.0f;

        vertices[5] = v2.x;
        vertices[6] = v2.y;
        vertices[7] = v2.z;

        vertices[8] = 1.0f;
        vertices[9] = 0.0f;

        vertices[10] = v1.x;
        vertices[11] = v1.y + 1.75;
        vertices[12] = v1.z;

        vertices[13] = 0.0f;
        vertices[14] = 1.0f;

        vertices[15] = v2.x;
        vertices[16] = v2.y + 1.75;
        vertices[17] = v2.z;

        vertices[18] = 1.0f;
        vertices[19] = 1.0f;

        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 1;
        indices[4] = 2;
        indices[5] = 3;

        setUpWall(ourShader);

        if (v1.x != v2.x) normal = glm::vec2(0, 1);
        else normal = glm::vec2(1, 0);
    };

    menuBottom() {};

    void draw(Shader& ourShader) {

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 0 , 0)); // translate it so it's at the center of the scene
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };

    void drawInGame(Camera camera, Shader& ourShader) {

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glm::mat4 viewMatrixInverse = glm::inverse(camera.GetViewMatrix());
        glm::vec3 cameraPositionWorldSpace = glm::vec3(viewMatrixInverse[3][0], viewMatrixInverse[3][1], viewMatrixInverse[3][2]);


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cameraPositionWorldSpace + camera.Front * 3.0f); // translate it so it's at the center of the scene

        float angle = atan(camera.Front.x / camera.Front.z) * 180.0f / 3.1415f;
        if (camera.Front.z > 0) angle -= 180;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };

private:
    // model data 
    float vertices[20];
    glm::vec2 normal;
    unsigned int indices[6];
    unsigned int VBO, VAO, EBO;
    unsigned int texture1;

    void setUpWall(Shader& ourShader) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindVertexArray(0);


        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        unsigned char* data;
        data = stbi_load("resources/Fotos_midi_maze/menuBottom.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        ourShader.use();
        glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);

    }




};

class menuRight {

public:

    glm::vec3 position = glm::vec3(0, 0, 0);

    menuRight(glm::vec3 v1, glm::vec3 v2, Shader& ourShader) {
        vertices[0] = v1.x + 4.35;
        vertices[1] = v1.y;
        vertices[2] = v1.z;

        vertices[3] = 0.0f;
        vertices[4] = 0.0f;

        vertices[5] = v2.x + 0.005;
        vertices[6] = v2.y;
        vertices[7] = v2.z;

        vertices[8] = 1.0f;
        vertices[9] = 0.0f;

        vertices[10] = v1.x + 4.35;
        vertices[11] = v1.y + 2.85;
        vertices[12] = v1.z;

        vertices[13] = 0.0f;
        vertices[14] = 1.0f;

        vertices[15] = v2.x + 0.005;
        vertices[16] = v2.y + 2.85;
        vertices[17] = v2.z;

        vertices[18] = 1.0f;
        vertices[19] = 1.0f;

        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 1;
        indices[4] = 2;
        indices[5] = 3;

        setUpWall(ourShader);

        if (v1.x != v2.x) normal = glm::vec2(0, 1);
        else normal = glm::vec2(1, 0);
    };

    menuRight() {};

    void draw(Shader& ourShader) {

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 1.75f, 0)); // translate it so it's at the center of the scene
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };

    void drawInGame(Camera camera, Shader& ourShader) {

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glm::mat4 viewMatrixInverse = glm::inverse(camera.GetViewMatrix());
        glm::vec3 cameraPositionWorldSpace = glm::vec3(viewMatrixInverse[3][0], viewMatrixInverse[3][1], viewMatrixInverse[3][2]);


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, (cameraPositionWorldSpace + camera.Front * 3.0f) + glm::vec3(0, 1.75f, 0)); // translate it so it's at the center of the scene

        float angle = atan(camera.Front.x / camera.Front.z) * 180.0f / 3.1415f;
        if (camera.Front.z > 0) angle -= 180;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };

private:
    // model data 
    float vertices[20];
    glm::vec2 normal;
    unsigned int indices[6];
    unsigned int VBO, VAO, EBO;
    unsigned int texture1;

    void setUpWall(Shader& ourShader) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindVertexArray(0);


        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        unsigned char* data;
        data = stbi_load("resources/Fotos_midi_maze/menuRight.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        ourShader.use();
        glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);

    }




};

class menuLeft {

public:

    glm::vec3 position = glm::vec3(0, 0, 0);

    menuLeft(glm::vec3 v1, glm::vec3 v2, Shader& ourShader) {
        vertices[0] = v1.x;
        vertices[1] = v1.y;
        vertices[2] = v1.z;

        vertices[3] = 0.0f;
        vertices[4] = 0.0f;

        vertices[5] = v2.x -7.52;
        vertices[6] = v2.y;
        vertices[7] = v2.z;

        vertices[8] = 1.0f;
        vertices[9] = 0.0f;

        vertices[10] = v1.x;
        vertices[11] = v1.y + 2.85;
        vertices[12] = v1.z;

        vertices[13] = 0.0f;
        vertices[14] = 1.0f;

        vertices[15] = v2.x - 7.52;
        vertices[16] = v2.y + 2.85;
        vertices[17] = v2.z;

        vertices[18] = 1.0f;
        vertices[19] = 1.0f;

        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 1;
        indices[4] = 2;
        indices[5] = 3;

        setUpWall(ourShader);

        if (v1.x != v2.x) normal = glm::vec2(0, 1);
        else normal = glm::vec2(1, 0);
    };

    menuLeft() {};

    void draw(Shader& ourShader) {

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.01f, 1.75f, 0)); // translate it so it's at the center of the scene
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };

    void drawInGame(Camera camera, Shader& ourShader) {

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glm::mat4 viewMatrixInverse = glm::inverse(camera.GetViewMatrix());
        glm::vec3 cameraPositionWorldSpace = glm::vec3(viewMatrixInverse[3][0], viewMatrixInverse[3][1], viewMatrixInverse[3][2]);


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, (cameraPositionWorldSpace + camera.Front * 3.0f) + glm::vec3(-0.01f, 1.75f, 0)); // translate it so it's at the center of the scene

        float angle = atan(camera.Front.x / camera.Front.z) * 180.0f / 3.1415f;
        if (camera.Front.z > 0) angle -= 180;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };

private:
    // model data 
    float vertices[20];
    glm::vec2 normal;
    unsigned int indices[6];
    unsigned int VBO, VAO, EBO;
    unsigned int texture1;

    void setUpWall(Shader& ourShader) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindVertexArray(0);


        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        unsigned char* data;
        data = stbi_load("resources/Fotos_midi_maze/menuLeft.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        ourShader.use();
        glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);

    }




};

class pointsToWin {

public:
    bool shown;

    // constructor, expects a filepath to a 3D model.
    pointsToWin(glm::vec3 v1, glm::vec3 v2, glm::vec3 camPosition, glm::vec3 front, Shader& ourShader, int index, bool show) {
        shown = show;
        double dist = 0.12;  //Está a 0.12 de la cámara

        vertices[0] = v1.x + 4.5 + (0.1 * index);
        vertices[1] = v1.y;
        vertices[2] = v1.z;

        vertices[3] = 0.0f;
        vertices[4] = 0.0f;

        vertices[5] = v2.x - 3.4 + (0.1 * index);
        vertices[6] = v2.y;
        vertices[7] = v2.z;

        vertices[8] = 1.0f;
        vertices[9] = 0.0f;

        vertices[10] = v1.x + 4.5 + (0.1 * index);
        vertices[11] = v1.y + 0.07;
        vertices[12] = v1.z;

        vertices[13] = 0.0f;
        vertices[14] = 1.0f;

        vertices[15] = v2.x - 3.4 + (0.1 * index);
        vertices[16] = v2.y + 0.07;
        vertices[17] = v2.z;

        vertices[18] = 1.0f;
        vertices[19] = 1.0f;

        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 1;
        indices[4] = 2;
        indices[5] = 3;

        setUpWall(ourShader, index);
        //setUpDespleg(v1, v2, ourShader);


        if (v1.x != v2.x) normal = glm::vec2(0, 1);
        else normal = glm::vec2(1, 0);
    };

    void setUp(Shader& ourShader, int index) {

        setUpWall(ourShader, index);
    }

    bool pause() {
        return shown;
    }

    void show() {
        shown = !shown;
    }

    pointsToWin() {};


    void draw(glm::vec3 camPosition, glm::vec3 front, float yaw, Shader& ourShader, float x1, float x2, float y1, float y2, int puntos) {
        if (shown) {
            glViewport(x1, y1, x2, y2);
            float dist = 0.12f;
            double ancho = 0.01;
            glm::vec3 frontPerp = glm::vec3(-front.z, 0, front.x);
            camPosition.x = camPosition.x + front.x * dist;
            camPosition.z = camPosition.z + front.z * dist;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, camPosition + glm::vec3(0, 4.02f + (0.052f * puntos), 0));
            //cout << front.x<<"\t"<<front.y<<"\t"<<front.z << endl;
            float angle = atan(front.x / front.z) * 180.0f / 3.1415f;
            if (front.z > 0) angle -= 180;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
            ourShader.setMat4("model", model);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            /*glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0, 0, 0)); // translate it so it's at the center of the scene
            ourShader.setMat4("model", model);*/
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

    };

private:
    // model data 
    float vertices[20];
    glm::vec2 normal;
    unsigned int indices[6];
    unsigned int VBO, VAO, EBO;
    unsigned int texture1;

    void setUpWall(Shader& ourShader, int index) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);


        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        string photo;
        if (index == 0) {
            photo = "resources/Fotos_midi_maze/faces/yellow.jpg";
        }
        else {
            photo = "resources/Fotos_midi_maze/faces/";
            photo = photo + colors[index - 1] + ".jpg";
        }
        /*if (numLives <= 0) photo = photo + "0hp.jpg";
        else if (numLives == 1) photo = photo + "1hp.jpg";
        else if (numLives == 2) photo = photo + "2hp.jpg";
        else photo = photo + "3hp.jpg";*/

        unsigned char* data;
        data = stbi_load(photo.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        ourShader.use();
        glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);

    };
};

class totalPoints {

public:
    bool shown;
    int totalEnemies;

    // constructor, expects a filepath to a 3D model.
    totalPoints(glm::vec3 camPosition, glm::vec3 front, Shader& ourShader,Enemy listaEnemigos) {
        totalEnemies = listaEnemigos.getNumEnemies();
        for (int i = 0; i <= totalEnemies; i++) {
            datos[i] = pointsToWin(glm::vec3(-4, -3, -4.2), glm::vec3(4, -3, -4.2), camPosition, front, ourShader, i, true);
        }
    };
    totalPoints() {};


    void draw(glm::vec3 camPosition, glm::vec3 front, float yaw, Shader& ourShader, float x1, float x2, float y1, float y2,Enemy listEnemies) {
        vector<int> allScores = listEnemies.getScores();

        for (int i = 0; i <= totalEnemies; i++) {
            datos[i].draw(camPosition, front, yaw, ourShader, x1, x2, y1, y2, allScores[i]);
        }
    };

private:

    // model data 
    pointsToWin datos[16];
    float vertices[20];
    glm::vec2 normal;

    /*void setUpWall(Shader& ourShader, int numLives) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        string photo = "resources/objects/smileys/yellow/yellow.jpg";
        /*if (numLives <= 0) photo = photo + "0hp.jpg";
        else if (numLives == 1) photo = photo + "1hp.jpg";
        else if (numLives == 2) photo = photo + "2hp.jpg";
        else photo = photo + "3hp.jpg";

        unsigned char* data;
        data = stbi_load(photo.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        ourShader.use();
        glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);

    };*/
};

class enemigoMuerto {
public:
    bool shown;

    enemigoMuerto(glm::vec3 v1, glm::vec3 v2, glm::vec3 camPosition, glm::vec3 front, Shader& ourShader, int index, bool show, string name) {
        shown = show;
        double dist = 0.12;  //Está a 0.12 de la cámara
        if (index > 4) {
            index = index - 5;
        }

        vertices[0] = v1.x + 4.45 + (0.3 * index);
        vertices[1] = v1.y;
        vertices[2] = v1.z;

        vertices[3] = 0.0f;
        vertices[4] = 0.0f;

        vertices[5] = v2.x - 3.33 + (0.3 * index);
        vertices[6] = v2.y;
        vertices[7] = v2.z;

        vertices[8] = 1.0f;
        vertices[9] = 0.0f;

        vertices[10] = v1.x + 4.45 + (0.3 * index);
        vertices[11] = v1.y + 0.26;
        vertices[12] = v1.z;

        vertices[13] = 0.0f;
        vertices[14] = 1.0f;

        vertices[15] = v2.x - 3.33 + (0.3 * index);
        vertices[16] = v2.y + 0.26;
        vertices[17] = v2.z;

        vertices[18] = 1.0f;
        vertices[19] = 1.0f;

        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 1;
        indices[4] = 2;
        indices[5] = 3;

        setUpWall(ourShader,name);
        //setUpDespleg(v1, v2, ourShader);


        if (v1.x != v2.x) normal = glm::vec2(0, 1);
        else normal = glm::vec2(1, 0);
    };

    void setUp(Shader& ourShader, string name) {

        setUpWall(ourShader, name);
    }

    bool pause() {
        return shown;
    }

    void show() {
        shown = !shown;
    }

    enemigoMuerto() {};


    void draw(glm::vec3 camPosition, glm::vec3 front, float yaw, Shader& ourShader, float x1, float x2, float y1, float y2, string name,int index) {
        if (shown) {
            glViewport(x1, y1, x2, y2);
            float dist = 0.12f;
            double ancho = 0.01;
            glm::vec3 frontPerp = glm::vec3(-front.z, 0, front.x);
            camPosition.x = camPosition.x + front.x * dist;
            camPosition.z = camPosition.z + front.z * dist;
            glm::mat4 model = glm::mat4(1.0f);
            if (index > 4) {
                model = glm::translate(model, camPosition + glm::vec3(0, 3.1f, 0));
            }
            else {
                model = glm::translate(model, camPosition + glm::vec3(0, 3.4f, 0));
            }
            //cout << front.x<<"\t"<<front.y<<"\t"<<front.z << endl;
            float angle = atan(front.x / front.z) * 180.0f / 3.1415f;
            if (front.z > 0) angle -= 180;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
            ourShader.setMat4("model", model);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

    };

private:
    // model data 
    float vertices[20];
    glm::vec2 normal;
    unsigned int indices[6];
    unsigned int VBO, VAO, EBO;
    unsigned int texture1;

    void setUpWall(Shader& ourShader,string name) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);


        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        string photo;
        photo = "resources/Fotos_midi_maze/kills/";
        photo = photo + name + ".jpg";

        unsigned char* data;
        data = stbi_load(photo.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        ourShader.use();
        glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);

    };
};

class jugadorMuertos {

public:
    bool shown;
    int totalPuntos;
    vector<string> listaMuertosJugador;
    int puntosAntes;

    // constructor, expects a filepath to a 3D model.
    jugadorMuertos(glm::vec3 camPosition, glm::vec3 front, Shader& ourShader, Enemy listaEnemigos) {
        totalPuntos= listaEnemigos.getPuntuacionJugador();
        listaMuertosJugador = listaEnemigos.getListaJugador();
        for (int i = 0; i < totalPuntos; i++) {
            datos[i] = enemigoMuerto(glm::vec3(-4, -3, -4.2), glm::vec3(4, -3, -4.2), camPosition, front, ourShader, i, true, listaMuertosJugador[i]);
        }
        puntosAntes = 0;
    };

    jugadorMuertos() {};

    void actualizar(glm::vec3 camPosition, glm::vec3 front, Shader& ourShader, Enemy listaEnemigos) {
        totalPuntos = listaEnemigos.getPuntuacionJugador();
        if (totalPuntos != puntosAntes) {
            listaMuertosJugador = listaEnemigos.getListaJugador();
            for (int i = puntosAntes; i < totalPuntos; i++) {
                datos[i] = enemigoMuerto(glm::vec3(-4, -3, -4.2), glm::vec3(4, -3, -4.2), camPosition, front, ourShader, i, true, listaMuertosJugador[i]);
            }
            puntosAntes = totalPuntos;
        }

    };


    void draw(glm::vec3 camPosition, glm::vec3 front, float yaw, Shader& ourShader, float x1, float x2, float y1, float y2, Enemy listEnemies) {
        vector<int> allScores = listEnemies.getScores();

        for (int i = 0; i < totalPuntos; i++) {
            datos[i].draw(camPosition, front, yaw, ourShader, x1, x2, y1, y2, listaMuertosJugador[i],i);
        }
    };

private:

    // model data 
    enemigoMuerto datos[10];
    /*void setUpWall(Shader& ourShader, int numLives) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        string photo = "resources/objects/smileys/yellow/yellow.jpg";
        /*if (numLives <= 0) photo = photo + "0hp.jpg";
        else if (numLives == 1) photo = photo + "1hp.jpg";
        else if (numLives == 2) photo = photo + "2hp.jpg";
        else photo = photo + "3hp.jpg";

        unsigned char* data;
        data = stbi_load(photo.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        ourShader.use();
        glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);

    };*/
};
