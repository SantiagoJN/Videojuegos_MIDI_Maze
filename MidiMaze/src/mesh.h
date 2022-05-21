#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader_m.h>

#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    unsigned int VAO;
    float maxX = 0.0f;
    float maxY = 0.0f;
    float maxZ = 0.0f;
    float minX = 100.0f;
    float minY = 100.0f;
    float minZ = 100.0f;



    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        for (Vertex& ver : this->vertices) {
            if (ver.Position.x > maxX) {
                maxX = ver.Position.x;
            }
            if (ver.Position.y > maxY) {
                maxY = ver.Position.y;
            }
            if (ver.Position.z > maxZ) {
                maxZ = ver.Position.z;
            }
            if (ver.Position.x < minX) {
                minX = ver.Position.x;
            }
            if (ver.Position.y < minY) {
                minY = ver.Position.y;
            }
            if (ver.Position.z < minZ) {
                minZ = ver.Position.z;
            }
        }

        //cout << "Values: " << maxX << " " << maxY << " " << maxZ << " " << minX << " " << minY << " " << minZ << endl;
        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    float getRadious() {
        return maxX - minX;
    }

    //compara si a es mayor que b
    bool compareNumbers(float a, float b) {
        return a - b > 1e-14;
    }

    bool checkCollision(glm::vec3 cameraPosition){
        // collision x-axis?
        bool collisionX = cameraPosition.x >= minX &&
            cameraPosition.x<= maxX;
        // collision y-axis?
        bool collisionY = cameraPosition.y >= minY &&
            cameraPosition.y<= maxY;
        // collision z-axis?
        bool collisionZ = cameraPosition.z >= minZ &&
            cameraPosition.z <= maxX;
        // collision only if on all axes
        if (collisionX && collisionY && collisionZ) {
            cout << "Posicion camara: " << cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << endl;
            cout << "Valores mapa: " << minX << " " << maxX << " " << minY << " " << maxY << " " << minZ << " " << maxZ << endl;
        }
        return collisionX && collisionY && collisionZ;

        /*float sqDist = 0.0000f;
        float limite = 0.0001f;
        if (cameraPosition.x < minX) {
            sqDist += (minX - cameraPosition.x) * (minX - cameraPosition.x);
            cout << "Resultado intermedio: " << sqDist << endl;
        }
        else if (cameraPosition.x > minX) {
            sqDist += (cameraPosition.x - maxX) * (cameraPosition.x - maxX) ;
            cout << "Resultado intermedio: " << sqDist << endl;
        }
        if (cameraPosition.y < minY) {
            sqDist += 1.0f * (minY - cameraPosition.y) * (minY - cameraPosition.y);
            cout << "Resultado intermedio: " << sqDist << endl;
        }
        else if (cameraPosition.y > maxY) {
            sqDist = sqDist + ((cameraPosition.y - maxY) * (cameraPosition.y - maxY));
            cout << "Resultado intermedio: " << sqDist << endl;
        }

        if (cameraPosition.z < minZ) {
            sqDist +=(minZ - cameraPosition.z) * (minZ - cameraPosition.z);
            cout << "Resultado intermedio: " << sqDist << endl;
        }
        else if (cameraPosition.z > maxZ) {
            sqDist += (cameraPosition.z - maxZ) * (cameraPosition.z - maxZ);
            cout << "Resultado intermedio: " << sqDist << endl;
        }

        if (sqDist < limite) {
            cout << "Posicion camara: " << cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << endl;
            cout << "Valores mapa: " << minX << " " << maxX << " " << minY << " " << maxY << " " << minZ << " " << maxZ << endl;
            cout << "Resultado: " << sqDist << endl;
        }
        return sqDist < limite;*/
    }
    // render the mesh
    void Draw(Shader& shader)
    {
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        // ids
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        // weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
        glBindVertexArray(0);
    }
};
#endif