#pragma once

#include <GLFW/glfw3.h>

// TODO - Gérer les clasvier AZERTY
// TODO - Ajouter un system de changement de touche

// Ensemble des touches du jeu
extern bool keys[512];

extern bool mouseButton[512];
extern bool mouseButtonRealeased[512];

void key_callback(GLFWwindow*, int key, int scancode, int action, int);
void mouse_button_callback(GLFWwindow*, int button, int action, int mods);

