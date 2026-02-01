#include "keys.h"

bool keys[512]{};

void key_callback(GLFWwindow*, int key, int scancode, int action, int)
{
    if (scancode < 0 || scancode >= 512) return;

    if (action == GLFW_PRESS)   keys[key] = true;
    if (action == GLFW_RELEASE) keys[key] = false;
}
