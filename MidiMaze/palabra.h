#pragma once
#include <letra.h>
class Palabra {

public:
    bool shown;
    vector<glm::vec4> buttons;
    string palabra;
    vector<letra> miLista;

    float ancho = 0.03f;
    float largo = 0.04f;

    // constructor, expects a filepath to a 3D model.
    Palabra(glm::vec3 v1Real, glm::vec3 v2Real,double xInicial, double y, Shader& ourShader, string palabra_) {
        palabra = palabra_;
        string show = palabra_;
        if (palabra_.size() > 15) {
            show[12] = '.';
            show[13] = '.';
            show[14] = '.';
            for (int i = 0; i < 15; i++) {
                setUpLetra(v1Real, v2Real, ourShader, i, show[i], xInicial, y);
                xInicial = xInicial + ancho - 0.005;
            }
        }
        else {
            int j = 0;
            for (int i = 0; i < palabra_.size()-4; i++) {
                setUpLetra(v1Real, v2Real, ourShader, i, palabra_[i], xInicial, y);
                xInicial = xInicial + ancho - 0.005;
                j++;
            }

            for (int i = j; i < 11; i++) {
                setUpLetra(v1Real, v2Real, ourShader, i, ' ', xInicial, y);
                xInicial = xInicial + ancho - 0.005;
                j++;
            }

            string maz = ".MAZ";
            for (int i = 0; i < maz.size(); i++) {
                setUpLetra(v1Real, v2Real, ourShader, j+i, maz[i], xInicial, y);
                xInicial = xInicial + ancho - 0.005;
            }
            

        }
        
        

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
        miLista.push_back(letra(glm::vec3(v1.x + x * totalx, v1.y + y * totaly, v1.z + 0.025), glm::vec3(v1.x + (x + ancho) * totalx, v1.y + y * totaly, v1.z + 0.025), static_cast<float>(v1.y + (y + largo) * totaly), ourShader, let));
    }



    void draw(Shader& ourShader, int index, bool selection) {
        for (int i = 0; i < miLista.size(); i++) miLista[i].draw(ourShader, index, selection);
    };

};