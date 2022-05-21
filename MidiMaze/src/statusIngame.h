#pragma once
class statusPlayer {

public:

    vector<glm::vec4> buttons;
    bool shown;

    // constructor, expects a filepath to a 3D model.
    statusPlayer(glm::vec3 v1, glm::vec3 v2, glm::vec3 camPosition, glm::vec3 front, Shader& ourShader, int numVidas,bool versionModernaGraficos) {
        shown = true;
        double ancho = 0.01;    //ancho/2
        double dist = 0.12;  //Est� a 0.12 de la c�mara
        double alto = 0.015;
        version = versionModernaGraficos;

        if (versionModernaGraficos) {
            glm::vec3 frontPerp = glm::vec3(-front.z, 0, front.x);
            camPosition.x = static_cast<float>(camPosition.x + front.x * dist);
            camPosition.z = static_cast<float>(camPosition.z + front.z * dist);

            v1 = glm::vec3(0, 0.035, 0);
            v2 = glm::vec3(0, 0.035, 0);
            v1.x = static_cast<float>(camPosition.x - ancho * frontPerp.x + 0.0562f * frontPerp.x);
            v2.x = static_cast<float>(camPosition.x + ancho * frontPerp.x + 0.0562f * frontPerp.x);
            v1.z = static_cast<float>(camPosition.z - ancho * frontPerp.z + 0.0562f * frontPerp.z);
            v2.z = static_cast<float>(camPosition.z + ancho * frontPerp.z + 0.0562f * frontPerp.z);
            v1 = glm::vec3(-ancho + 0.0562f, 0.035, 0);
            v2 = glm::vec3(ancho+ 0.0562f, 0.035, 0);
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
            vertices[11] = static_cast<float>(v1.y + alto);
            vertices[12] = v1.z;

            vertices[13] = 0.0f;
            vertices[14] = 1.0f;

            vertices[15] = v2.x;
            vertices[16] = static_cast<float>(v2.y + alto);
            vertices[17] = v2.z;

            vertices[18] = 1.0f;
            vertices[19] = 1.0f;

            indices[0] = 0;
            indices[1] = 1;
            indices[2] = 2;
            indices[3] = 1;
            indices[4] = 2;
            indices[5] = 3;
        }
        else {
            vertices[0] = v1.x + 3.33f;
            vertices[1] = v1.y;
            vertices[2] = v1.z;

            vertices[3] = 0.0f;
            vertices[4] = 0.0f;

            vertices[5] = v2.x - 3.9f;
            vertices[6] = v2.y;
            vertices[7] = v2.z;

            vertices[8] = 1.0f;
            vertices[9] = 0.0f;

            vertices[10] = v1.x + 3.33f;
            vertices[11] = v1.y + 0.65f;
            vertices[12] = v1.z;

            vertices[13] = 0.0f;
            vertices[14] = 1.0f;

            vertices[15] = v2.x - 3.9f;
            vertices[16] = v2.y + 0.65f;
            vertices[17] = v2.z;

            vertices[18] = 1.0f;
            vertices[19] = 1.0f;

            indices[0] = 0;
            indices[1] = 1;
            indices[2] = 2;
            indices[3] = 1;
            indices[4] = 2;
            indices[5] = 3;
        }

        setUpWall(ourShader,numVidas);
        //setUpDespleg(v1, v2, ourShader);


        if (v1.x != v2.x) normal = glm::vec2(0, 1);
        else normal = glm::vec2(1, 0);
    };

    void setUp(Shader& ourShader, int numLives) {
        
        setUpWall(ourShader,numLives);
    }

    bool pause() {
        return shown;
    }

    void show() {
        shown = !shown;
    }

    statusPlayer() {};


    void draw(glm::vec3 camPosition, glm::vec3 front, float yaw, Shader& ourShader, float x1, float x2, float y1, float y2) {
        if (shown) {
            glViewport(static_cast<GLint>(x1), static_cast<GLint>(y1), static_cast<GLsizei>(x2), static_cast<GLsizei>(y2));
            float dist = 0.12f;
            double ancho = 0.01;
            glm::vec3 frontPerp = glm::vec3(-front.z, 0, front.x);
            camPosition.x = camPosition.x + front.x * dist;
            camPosition.z = camPosition.z + front.z * dist;
            glm::mat4 model = glm::mat4(1.0f);
            float value = 0;
            if (!version) {
                value = 3.98f;
            }
            model = glm::translate(model, camPosition + glm::vec3(0,value,0));
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
    bool version;
    void setUpWall(Shader& ourShader, int numLives) {
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
        string photo = "resources/Fotos_midi_maze/";
        if (numLives <= 0) photo = photo + "0hp.jpg";
        else if(numLives == 1) photo = photo + "1hp.jpg";
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

    };
};
