#pragma once
class gameLeaver {

public:

    vector<glm::vec4> buttons;
    bool shown;

    // constructor, expects a filepath to a 3D model.
    gameLeaver(glm::vec3 camPosition,glm::vec3 front, Shader& ourShader) {
        shown = false;
        double ancho = 0.04;    //ancho/2
        double dist = 0.12;  //Est� a 0.12 de la c�mara

        glm::vec3 frontPerp = glm::vec3(-front.z, 0, front.x);
        camPosition.x = static_cast<float>(camPosition.x + front.x * dist);
        camPosition.z = static_cast<float>(camPosition.z + front.z * dist);

        cout << "Punto:" << camPosition.x << "," << camPosition.z << endl;
        glm::vec3 v1(0, -0.015, 0);
        glm::vec3 v2(0, -0.015, 0);
        v1.x = static_cast<float>(camPosition.x - 0.04f * frontPerp.x);
        v2.x = static_cast<float>(camPosition.x + 0.04f * frontPerp.x);
        v1.z = static_cast<float>(camPosition.z - 0.04f * frontPerp.z);
        v2.z = static_cast<float>(camPosition.z + 0.04f * frontPerp.z);
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
        vertices[11] = static_cast<float>(v1.y + 0.03f);
        vertices[12] = v1.z;

        vertices[13] = 0.0f;
        vertices[14] = 1.0f;

        vertices[15] = v2.x;
        vertices[16] = static_cast<float>(v2.y + 0.03f);
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
        //setUpDespleg(v1, v2, ourShader);

        buttons.push_back(glm::vec4(0.31, 0.47, 0.56, 0.62));
        buttons.push_back(glm::vec4(0.52, 0.68, 0.56, 0.62));

        if (v1.x != v2.x) normal = glm::vec2(0, 1);
        else normal = glm::vec2(1, 0);
    };

    bool pause() {
        return shown;
    }

    void show() {
        shown = !shown;
    }

    gameLeaver() {};

    int checkButton(double xPos, double yPos, Shader& ourShader) {
        for (int i = 0; i < buttons.size(); i++) {
            if (xPos >= buttons[i].x && xPos <= buttons[i].y && yPos >= buttons[i].z && yPos <= buttons[i].w) {
                SoundEngine->play2D("resources/effects/plik.mp3", false);
                if (i == 0) {
                    return 0;
                }
                else return 1;
            }
        }
        return -1;
    };

    void draw(Shader& ourShader) {
        if (shown) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0, 0, 0)); // translate it so it's at the center of the scene
            ourShader.setMat4("model", model);
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
        data = stbi_load("resources/Fotos_midi_maze/quit.jpg", &width, &height, &nrChannels, 0);
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
