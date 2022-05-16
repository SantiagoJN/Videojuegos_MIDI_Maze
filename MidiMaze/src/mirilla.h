#pragma once
class mirilla {

public:

    Model punto;
    float scale = 0.0005f;

    // constructor, expects a filepath to a 3D model.
    mirilla(string path) {
        
        punto = Model(path);
    };


    void draw(glm::vec3 camPosition, glm::vec3 front, Shader& ourShader,float x1,float x2, float y1, float y2) {
            
            glViewport(x1, y1, x2, y2);
            double ancho = 0;    //ancho/2
            double dist = 0.1;  //Está a 0.12 de la cámara

            glm::vec3 frontPerp = glm::vec3(-front.z, 0, front.x);
            camPosition.x = static_cast<float>(camPosition.x + front.x * dist);
            camPosition.z = static_cast<float>(camPosition.z + front.z * dist);

            glm::vec3 v1(0, 0, 0);
            v1.x = static_cast<float>(camPosition.x - ancho * frontPerp.x);
            v1.z = static_cast<float>(camPosition.z - ancho * frontPerp.z);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, v1);
            model = glm::scale(model, glm::vec3(scale, scale, scale));
            ourShader.setMat4("model", model);
            punto.Draw(ourShader);

    };

private:
    // model data 
    float vertices[20];
    glm::vec2 normal;
    unsigned int indices[6];
    unsigned int VBO, VAO, EBO;
    unsigned int texture1;
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

    };
};
