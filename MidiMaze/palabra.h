#pragma once
#include <letra.h>
class Palabra {

public:
    bool shown;
    vector<glm::vec4> buttons;
    string palabra;
    char letras[12];
    letra miLista[12];

    float ancho = 0.03;
    float largo = 0.04;

    // constructor, expects a filepath to a 3D model.
    Palabra(glm::vec3 v1Real, glm::vec3 v2Real,double xInicial, double y, Shader& ourShader, string palabra) {
        for (int i = 0; i < 12; i++) {
            letras[i] = palabra[i];
            setUpLetra(v1Real, v2Real, ourShader,i, letras[i], xInicial, y);
            xInicial = xInicial + ancho-0.005;
        }
        shown = false;
        

    };

    Palabra() {};


    void buttonCalled() {
        shown = !shown;
    }

    bool getShown() {
        return shown;
    }

    void setUpLetra(glm::vec3 v1, glm::vec3 v2, Shader& ourShader, int indice, char let, double x, double y) {
        double totalx = v2.x - v1.x;
        double totaly = 3 - v1.y;
        miLista[indice] = letra(glm::vec3(v1.x + x * totalx, v1.y + y * totaly, v1.z + 0.025), glm::vec3(v1.x + (x + ancho) * totalx, v1.y + y * totaly, v1.z + 0.025), v1.y + (y + largo) * totaly, ourShader, let);
    }



    void draw(Shader& ourShader) {
        for (int i = 0; i < 12; i++) miLista[i].draw(ourShader);
    };

};