#pragma once

#include <GLFW/glfw3.h>

// Ensemble des touches du jeu
extern bool keys[512];

void key_callback(GLFWwindow*, int key, int scancode, int action, int);

