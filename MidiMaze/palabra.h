#pragma once
#include <letra.h>
#include <diccionario.h>
class Palabra {

public:
    bool shown;
    vector<glm::vec4> buttons;
    string palabra;
    string show;
    vector<letra> miLista;
    diccionario dicc;

    float ancho = 0.03f;
    float largo = 0.04f;

    // constructor, expects a filepath to a 3D model.
    Palabra(glm::vec3 v1Real, glm::vec3 v2Real,double xInicial, double y, Shader& ourShader, string palabra_, diccionario dicc_) : dicc(dicc_) {
        palabra = palabra_;
        show = "...............";
        for (int i = 0; i < show.size(); i++) {
            if (i >= palabra.size()) break;
            else show[i] = palabra[i];
        }
        if (palabra_.size() > 15) {
            show[12] = '.';
            show[13] = '.';
            show[14] = '.';
        }
        else if (palabra.size() < 15) {
            for (int i = palabra.size() - 1; i < 12; i++) {
                show[i] = ' ';
            }
            show[12] = '.';
            show[14] = 'M';
            show[13] = 'A';
            show[14] = 'Z';
        }
        
        

    };


    void buttonCalled() {
        shown = !shown;
    }

    bool getShown() {
        return shown;
    }



    //Shader& ourShader, int indexL, int indexP, bool selection, int numLetra, int numNumero, bool guion_, bool punto_
    void draw(Shader& ourShader, int index, bool selection) {
        for (int i = 0; i < 15; i++) {
            char caract = show[i];
            int numLetra = -1;
            int numNumero = -1;
            bool guion_ = false;
            bool punto_ = false;
            bool espacio = false;
            if (caract >= 'A' && caract <= 'Z') {
                caract = caract + 32;
            }
            if (caract == '_') guion_ = true;
            else if (caract == '.') punto_ = true;
            else if (caract >= '0' && caract <= '9') numNumero = caract - '0';
            else if (caract >= 'a' && caract <= 'z') numLetra = caract - 'a';
            else  espacio = true;
            dicc.draw(ourShader, i, index, selection, numLetra, numNumero, guion_, punto_, espacio);
        }
        
    };

};