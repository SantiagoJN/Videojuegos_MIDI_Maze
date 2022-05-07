#pragma once
class Wall
{
public:
    // model data 
    float vertices[20];
    glm::vec2 normal;
    unsigned int indices[6];
    unsigned int VBO, VAO, EBO;
    unsigned int texture1;
    bool suelo = false;

    // constructor, expects a filepath to a 3D model.
    Wall(glm::vec3 v1, glm::vec3 v2, Shader& ourShader) {
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
        vertices[11] = v1.y+2;
        vertices[12] = v1.z;

        vertices[13] = 0.0f;
        vertices[14] = 1.0f;

        vertices[15] = v2.x;
        vertices[16] = v2.y+2;
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


    // constructor, expects a filepath to a 3D model.
    Wall(float size, float dim, Shader& ourShader) {
        suelo = true;
        vertices[0] = size * dim/2;
        vertices[1] = -0.5;
        vertices[2] = size * dim/2;

        vertices[3] = 0.0f;
        vertices[4] = 0.0f;

        vertices[5] = size * dim/2;
        vertices[6] = -0.5;
        vertices[7] = size * -dim/2;

        vertices[8] = 1.0f;
        vertices[9] = 0.0f;

        vertices[10] = size * -dim/2;
        vertices[11] = -0.5;
        vertices[12] = size * dim/2;

        vertices[13] = 0.0f;
        vertices[14] = 1.0f;

        vertices[15] = size * -dim/2;
        vertices[16] = -0.5;
        vertices[17] = size * -dim/2;

        vertices[18] = 1.0f;
        vertices[19] = 1.0f;

        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 1;
        indices[4] = 2;
        indices[5] = 3;

        setUpWall(ourShader);
    };

    Wall() {};

    

	
    bool intersect(float x1, float z1, float x2, float z2,bool isBullet,float radius) {
        // collision x-axis?
        float normalize = 0.5f;
        float normalize2 = 0.0f;
        if (isBullet) {
            normalize = radius;
        }
        // collision z-axis?
        bool collisionX = ((x2 >= vertices[0] - normalize && x1 <= vertices[0] + normalize) || (x1 >= vertices[0] - normalize && x2 <= vertices[0] + normalize)) || ((x1 >= vertices[0] &&
            x1 <= vertices[5]) || (x1 >= vertices[5] && x1 <= vertices[0])) || ((x2 >= vertices[5] - normalize && x1 <= vertices[5] + normalize) || (x1 >= vertices[5] - normalize && x2 <= vertices[5] + normalize));
        // collision z-axis?
        bool collisionZ = ((z2 >= vertices[2] - normalize && z1 <= vertices[2] + normalize) || (z1 >= vertices[2] - normalize && z2 <= vertices[2] + normalize)) || ((z1 >= vertices[2] &&
            z1 <= vertices[7]) || (z1 >= vertices[7] && z1 <= vertices[2])) || ((z2 >= vertices[7] - normalize && z1 <= vertices[7] + normalize) || (z1 >= vertices[7] - normalize && z2 <= vertices[7] + normalize));

        if (collisionX && collisionZ) {
            //cout << vertices[0] << " " << vertices[5] << " " << vertices[2] << " " << vertices[7] << endl;
            //cout << x1 << " " << x2 << " " << z1 << " " << z2 << endl;
            //cout << collisionX1 << collisionX2 << collisionX3 << collisionX4 << endl;
        }
        // collision only if on all axes
        return collisionX && collisionZ;
    }

    
    bool between(glm::vec2 A, glm::vec2 B) {

        glm::vec2 dir = glm::normalize(B-A);
        float denom = dir.x * normal.x + dir.y * normal.y;
        if (denom < 1e-10 && denom >= 0.0) {
            //If the dot product is very small, the vectors are almost parallel, 
            // so it doesn't intersect
            return false;
        }
        glm::vec2 n = glm::vec2(vertices[0], vertices[2]) - A;
        float num = n.x * normal.x + n.y * normal.y;
        float t = num / denom;
        float distance = t;
        //cout << "distMuroA " << t << endl;
        if (t > 0.0f) { //Ray intersects with plane, let's see if it intersects with square
            //cout << "distMuro " << t << endl;
            glm::vec2 intersectPoint = A + distance * dir;
            float x = intersectPoint.x;
            float y = intersectPoint.y;
            bool inside = ((x >= vertices[0] && x <= vertices[5]) || (x <= vertices[0] && x >= vertices[5])) && ((y >= vertices[2] && y <= vertices[7]) || (y <= vertices[2] && y >= vertices[7]));
            //cout << "length " << glm::length(A - B) << endl;
            if (inside) {
                float l = glm::length(A - B);
                return l > distance;
            }
        }
        else return false;

    }


    void draw(Shader& ourShader) {
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 0, 0)); // translate it so it's at the center of the scene
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };

private:

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
        if(!suelo) data = stbi_load("resources/textures/wall.jpg", &width, &height, &nrChannels, 0);
        else data = stbi_load("resources/textures/floor.jpg", &width, &height, &nrChannels, 0);
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