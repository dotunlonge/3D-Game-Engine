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
    
    // Mouse delta (for camera control)
    static glm::vec2 GetMouseDelta();
    static void SetMouseDelta(float x, float y);
    static void ResetMouseDelta();

    static void SetWindow(GLFWwindow* window) { s_Window = window; }
    static void SetCursorEnabled(bool enabled);

private:
    static GLFWwindow* s_Window;
    static glm::vec2 s_LastMousePosition;
    static glm::vec2 s_MouseDelta;
    static bool s_FirstMouse;
};

