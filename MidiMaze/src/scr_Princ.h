#pragma once
#include <desplegable.h>
#include <configuracionAvanzada.h>

class Princip {

public: 
        
        Despleg desplegable;
        confAvanzada config;
        vector<glm::vec4> buttons;
		glm::vec3 position = glm::vec3(0, 0, 0);

        // constructor, expects a filepath to a 3D model.
        Princip(glm::vec3 v1, glm::vec3 v2, Shader& ourShader, boolean menu) {
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
            vertices[11] = v1.y + 6;
            vertices[12] = v1.z;

            vertices[13] = 0.0f;
            vertices[14] = 1.0f;

            vertices[15] = v2.x;
            vertices[16] = v2.y + 6;
            vertices[17] = v2.z;

            vertices[18] = 1.0f;
            vertices[19] = 1.0f;

            indices[0] = 0;
            indices[1] = 1;
            indices[2] = 2;
            indices[3] = 1;
            indices[4] = 2;
            indices[5] = 3;

            setUpWall(ourShader,menu);
            setUpDespleg(v1,v2,ourShader);
            setUpConfigAvanzada(v1, v2, ourShader);

            buttons.push_back(glm::vec4(0.177, 0.273, 0, 0.05));
            buttons.push_back(glm::vec4(0.187, 0.38, 0.245, 0.51));
            buttons.push_back(glm::vec4(0.085, 0.14, 0, 0.05));

            if (v1.x != v2.x) normal = glm::vec2(0, 1);
            else normal = glm::vec2(1, 0);
        };


        Princip() {};


        bool getRegenSpeed() {
            return desplegable.settings.regen.getSpeed();
        }

        bool getReviveSpeed() {
            return desplegable.settings.revive.getSpeed();
        }

        bool getReloadSpeed() {
            return desplegable.settings.reload.getSpeed();
        }

        int getVeryDumb() {
            return desplegable.settings.veryDumb.getNEnemies();
        }

        int getPlainDumb() {
            return desplegable.settings.plainDumb.getNEnemies();
        }

        int getNotDumb() {
            return desplegable.settings.notDumb.getNEnemies();
        }

        int getNumVidas() {
            return desplegable.settings.vidas.getNumVidas();
        }

        string getMapName() {
            return desplegable.maps.getSelection();
        }

        void setUpDespleg(glm::vec3 v1, glm::vec3 v2, Shader& ourShader) {
            double totalx = v2.x - v1.x;
            double totaly = 3 - v1.y;
            desplegable = Despleg(glm::vec3(v1.x + 0.175f * totalx, v1.y + 0.7f * totaly, v1.z + 0.01f), glm::vec3(v1.x + 0.46f * totalx, v1.y + 0.7f * totaly, v1.z + 0.01f), static_cast<float>(v1.y + 0.94f * totaly), ourShader, v1, v2);
        }

        void setUpConfigAvanzada(glm::vec3 v1, glm::vec3 v2, Shader& ourShader) {
            double totalx = v2.x - v1.x;
            double totaly = 3 - v1.y;
            config = confAvanzada(glm::vec3(v1.x + 0.061f * totalx, v1.y + 0.287f * totaly, v1.z + 0.01f), glm::vec3(v1.x + 0.54f * totalx, v1.y + 0.2867f * totaly, v1.z + 0.01f), static_cast<float>(v1.y + 0.755f * totaly), ourShader, v1, v2);
        }

        bool checkButton(double xPos, double yPos, Shader& ourShader, GLFWwindow* window) {
            if(config.getShown()) config.checkButton(xPos, yPos, ourShader, window);
            if (desplegable.getShown()) desplegable.checkButton(xPos, yPos, ourShader, window);
            for (int i = 0; i < buttons.size(); i++) {
                if (!desplegable.maps.getShown() && !desplegable.settings.getShown()) {
                    if (xPos >= buttons[i].x && xPos <= buttons[i].y && yPos >= buttons[i].z && yPos <= buttons[i].w) {
                        if (i == 2 ) {
                            config.buttonCalled();
                            SoundEngine->play2D("resources/effects/plik.mp3", false);
                            if(desplegable.shown) desplegable.buttonCalled();
                        }
                        else if(i != 2) {
                            cout << "BOTON PULSADO" << endl;
                            
                            if (!config.shown) {
                                desplegable.buttonCalled();
                                SoundEngine->play2D("resources/effects/plik.mp3", false);
                            }
                            else if (config.shown && i == 0) {
                                config.buttonCalled();
                                SoundEngine->play2D("resources/effects/plik.mp3", false);
                            }
                        }
                    }
                }
            }
            if (desplegable.maps.getShown()) desplegable.maps.checkButton(xPos, yPos, ourShader);
            if (desplegable.start.getShown()) {
                int which = desplegable.start.checkButton(xPos, yPos);
                if (which == 1) {
                    int total = desplegable.settings.veryDumb.getNEnemies() + desplegable.settings.notDumb.getNEnemies() + desplegable.settings.plainDumb.getNEnemies();
                    if(total > 0) SoundEngine->play2D("resources/effects/iniciopartida.mp3", false);
                    else SoundEngine->play2D("resources/effects/hitmarker.mp3", false);
                    return total > 0;
                }
                else if (which == 2) {
                    desplegable.start.buttonCalled();
                    desplegable.settings.buttonCalled();
                }
            }
            if (desplegable.settings.getShown()) {
                desplegable.settings.checkButton(xPos, yPos, ourShader);
            }
            return false;
        };


        void predSettings() {
            desplegable.maps.selection = 0;
            desplegable.settings.restart();
        }


        void draw(Shader& ourShader) {

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0, 0, 0)); // translate it so it's at the center of the scene
            ourShader.setMat4("model", model);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            desplegable.draw(ourShader);
            config.draw(ourShader);
        };

		void drawInGame(Camera camera , Shader& ourShader) {

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);

            /*double ancho = 0;    //ancho/2
            double dist = 0.1;  //Está a 0.12 de la cámara

            glm::vec3 frontPerp = glm::vec3(-front.z, 0, front.x);
            camPosition.x = static_cast<float>(camPosition.x + front.x * dist);
            camPosition.z = static_cast<float>(camPosition.z + front.z * dist);

            glm::vec3 v1(0, 0, 0);
            v1.x = static_cast<float>(camPosition.x - ancho * frontPerp.x);
            v1.z = static_cast<float>(camPosition.z - ancho * frontPerp.z);*/

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
            desplegable.draw(ourShader);
            config.draw(ourShader);
        };
    private:
        // model data 
        float vertices[20];
        glm::vec2 normal;
        unsigned int indices[6];
        unsigned int VBO, VAO, EBO;
        unsigned int texture1;
        void setUpWall(Shader& ourShader,bool menu) {
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
            if (menu) {
                data = stbi_load("resources/Fotos_midi_maze/inicio.jpg", &width, &height, &nrChannels, 0);
            }
            else {
                data = stbi_load("resources/Fotos_midi_maze/inicio.png", &width, &height, &nrChannels, 0);
            }
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