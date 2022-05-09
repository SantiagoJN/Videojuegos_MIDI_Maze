#pragma once
class diccionario {

public:
    bool shown;
    vector<letra> letras;
    vector<letra> numeros;
    letra guion;
    letra punto;
    letra espacio;

    float ancho = 0.03f;
    float largo = 0.04f;

    // constructor, expects a filepath to a 3D model.
    diccionario(glm::vec3 v1Real, glm::vec3 v2Real, double xInicial, double y, Shader& ourShader) {
        

        setUpLetra(v1Real, v2Real, ourShader,  xInicial, y);
    };

    diccionario() {};



    void setUpLetra(glm::vec3 v1, glm::vec3 v2, Shader& ourShader, double x, double y) {
        double totalx = v2.x - v1.x;
        double totaly = 3 - v1.y;
        for (int i = 'a'; i <= 'z'; i++) {
            char let = i;
            letras.push_back(letra(glm::vec3(v1.x + x * totalx, v1.y + y * totaly, v1.z + 0.025), glm::vec3(v1.x + (x + ancho) * totalx, v1.y + y * totaly, v1.z + 0.025), static_cast<float>(v1.y + (y + largo) * totaly), ourShader, let));
            
        }
        for (int i = '0'; i <= '9'; i++) {
            char let = i;
            numeros.push_back(letra(glm::vec3(v1.x + x * totalx, v1.y + y * totaly, v1.z + 0.025), glm::vec3(v1.x + (x + ancho) * totalx, v1.y + y * totaly, v1.z + 0.025), static_cast<float>(v1.y + (y + largo) * totaly), ourShader, let));
        }
        guion = letra(glm::vec3(v1.x + x * totalx, v1.y + y * totaly, v1.z + 0.025), glm::vec3(v1.x + (x + ancho) * totalx, v1.y + y * totaly, v1.z + 0.025), static_cast<float>(v1.y + (y + largo) * totaly), ourShader, '_');
        punto = letra(glm::vec3(v1.x + x * totalx, v1.y + y * totaly, v1.z + 0.025), glm::vec3(v1.x + (x + ancho) * totalx, v1.y + y * totaly, v1.z + 0.025), static_cast<float>(v1.y + (y + largo) * totaly), ourShader, '.');
        espacio = letra(glm::vec3(v1.x + x * totalx, v1.y + y * totaly, v1.z + 0.025), glm::vec3(v1.x + (x + ancho) * totalx, v1.y + y * totaly, v1.z + 0.025), static_cast<float>(v1.y + (y + largo) * totaly), ourShader, ' ');
    };

    void draw(Shader& ourShader, int indexL, int indexP, bool selection, int numLetra, int numNumero, bool guion_, bool punto_, bool espacio_) {
        if (numLetra != -1) {
            letras[numLetra].draw(ourShader, indexL, indexP, selection);
        }
        else if (numNumero != -1) {
            numeros[numNumero].draw(ourShader, indexL, indexP, selection);
        }
        else if (guion_) {
            guion.draw(ourShader, indexL, indexP, selection);
        }
        else if (punto_) {
            punto.draw(ourShader, indexL, indexP, selection);
        }
        else if (espacio_) {
            espacio.draw(ourShader, indexL, indexP, selection);
        }
    }


};