#pragma once
#include <screen.h>

class Menu
{
public:
	vector<Screen> scr;
	vector<bool> shown;

	Menu(Shader& ourShader) {
		vector<glm::vec4> butt;							//Menu principal
		butt.push_back(glm::vec4(0.177, 0.273, 0, 0.05));
		Screen menu(glm::vec3(-4, -3, -4.2), glm::vec3(4, -3, -4.2), ourShader, butt, "resources/Fotos_midi_maze/inicio.png");
		shown.push_back(true);
		scr.push_back(menu);
	};


	void checkButton(double xPos, double yPos) {
		for (int i = 0; i < scr.size(); i++) {
			int which=scr[i].checkButton(xPos, yPos);
			//shown
		}
	}

};