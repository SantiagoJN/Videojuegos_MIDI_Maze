#pragma once
#include <palabra.h>
#include <filesystem>
#include <diccionario.h>
class mapSelector {

public:
    bool shown;
    vector<glm::vec4> buttons;
    int selection = 0;
    vector<Palabra> pal;
    int currentIndex;
    diccionario dicc;

    float ancho = 0.03f;
    float largo = 0.04f;

    // constructor, expects a filepath to a 3D model.
    mapSelector(glm::vec3 v1, glm::vec3 v2, float ySup, Shader& ourShader, glm::vec3 v1Real, glm::vec3 v2Real) {
        shown = false;
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
        vertices[11] = ySup;
        vertices[12] = v1.z;

        vertices[13] = 0.0f;
        vertices[14] = 1.0f;

        vertices[15] = v2.x;
        vertices[16] = ySup;
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
        double x = 0.175 - ancho * 2;
        double y = 0.497;
        dicc = diccionario(v1Real, v2Real, x, y, ourShader);
        int i = 0;
        for (const auto& file : std::filesystem::directory_iterator("resources/maps/")) {
            string a = file.path().filename().string();
            if (a.substr(a.size() - 4) == ".MAZ") {
                double x = 0.175 - ancho*2;
                double y = 0.497;// - 0.04f * i;
                setUpPalabra(v1Real, v2Real, x, y, ourShader, file.path().filename().string(), i);
                i++;
                currentIndex = 0;
            }
            
        }
        
        
        cout << "Map selector" << endl;
        buttons.push_back(glm::vec4(0.7, 0.93, 0.605, 0.65));
        buttons.push_back(glm::vec4(0.7, 0.93, 0.52, 0.575));
        buttons.push_back(glm::vec4(0.547, 0.588, 0.415, 0.45));
        buttons.push_back(glm::vec4(0.547, 0.588, 0.825, 0.865));

        buttons.push_back(glm::vec4(0.115, 0.49, 0.46, 0.495));
        buttons.push_back(glm::vec4(0.115, 0.49, 0.5, 0.536));
        buttons.push_back(glm::vec4(0.115, 0.49, 0.54, 0.576));
        buttons.push_back(glm::vec4(0.115, 0.49, 0.581, 0.616));
        buttons.push_back(glm::vec4(0.115, 0.49, 0.625, 0.655));
        buttons.push_back(glm::vec4(0.115, 0.49, 0.665, 0.698));
        buttons.push_back(glm::vec4(0.115, 0.49, 0.705, 0.738));
        buttons.push_back(glm::vec4(0.115, 0.49, 0.745, 0.778));
        buttons.push_back(glm::vec4(0.115, 0.49, 0.785, 0.82));

        if (v1.x != v2.x) normal = glm::vec2(0, 1);
        else normal = glm::vec2(1, 0);
    };

    mapSelector() {};

    string getSelection() {
        return pal[selection].palabra;
    }

    void buttonCalled() {
        shown = !shown;
    }

    bool getShown() {
        return shown;
    }


    bool checkButton(double xPos, double yPos, Shader& ourShader) {
        for (int i = 0; i < buttons.size(); i++) {
            if (xPos >= buttons[i].x && xPos <= buttons[i].y && yPos >= buttons[i].z && yPos <= buttons[i].w) {
                SoundEngine->play2D("resources/effects/plik.mp3", false);
                if (i == 0) shown = false;
                else if (i == 1) {
                    shown = false;
                }
                else if (i == 3) {
                    if (currentIndex + 9 < pal.size()) currentIndex++;
                }
                else if(i==2) {
                    if (currentIndex > 0) currentIndex--;
                }
                else if (i > 3) {
                    int map = i - 4;
                    if(currentIndex + map < pal.size())selection = currentIndex + map;
                    
                    cout << getSelection() << endl;
                }
            }
        }
        return false;
    };

    void setUpPalabra(glm::vec3 v1, glm::vec3 v2,double x,double y, Shader& ourShader,string palabra, int index) {
        double totalx = v2.x - v1.x;
        double totaly = 3 - v1.y;
        pal.push_back(Palabra(v1, v2, x, y, ourShader, palabra, dicc));
    }


    void draw(Shader& ourShader) {
        if (shown) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0, 0, 0)); // translate it so it's at the center of the scene
            ourShader.setMat4("model", model);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            int gap = currentIndex + 9;
            if (gap > pal.size()) gap = static_cast<int>(pal.size());
            for (int i = currentIndex; i < gap; i++) {
                pal[i].draw(ourShader,i-currentIndex,false);
            }
            pal[selection].draw(ourShader, selection, true);
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

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
        data = stbi_load("resources/Fotos_midi_maze/mapSelect/selectorwindow2.jpg", &width, &height, &nrChannels, 0);
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