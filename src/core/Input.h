#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Input {
public:
    static bool IsKeyPressed(int keycode);
    static bool IsMouseButtonPressed(int button);
    static glm::vec2 GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();

    static void SetWindow(GLFWwindow* window) { s_Window = window; }

private:
    static GLFWwindow* s_Window;
};

