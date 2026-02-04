#include "keys.h"
#include <iostream>

bool keys[512]{};

bool mouseButton[512]{};
bool mouseButtonRealeased[512]{1};

void key_callback(GLFWwindow*, int key, int scancode, int action, int)
{
    if (scancode < 0 || scancode >= 512) return;

    if (action == GLFW_PRESS)   keys[key] = true;
    if (action == GLFW_RELEASE) keys[key] = false;
}

void mouse_button_callback(GLFWwindow*, int button, int action, int mods)
{
    if (action == GLFW_PRESS){
      mouseButton[button] = true;
    } 

    if (action == GLFW_RELEASE){
      mouseButton[button] = false;
      mouseButtonRealeased[button] = true;
    } 
}
