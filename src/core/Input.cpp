#include "Input.h"

GLFWwindow* Input::s_Window = nullptr;
glm::vec2 Input::s_LastMousePosition = glm::vec2(0.0f);
glm::vec2 Input::s_MouseDelta = glm::vec2(0.0f);
bool Input::s_FirstMouse = true;

bool Input::IsKeyPressed(int keycode) {
    if (!s_Window) return false;
    auto state = glfwGetKey(s_Window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button) {
    if (!s_Window) return false;
    return glfwGetMouseButton(s_Window, button) == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition() {
    if (!s_Window) return glm::vec2(0.0f);
    double xpos, ypos;
    glfwGetCursorPos(s_Window, &xpos, &ypos);
    return glm::vec2((float)xpos, (float)ypos);
}

float Input::GetMouseX() {
    return GetMousePosition().x;
}

float Input::GetMouseY() {
    return GetMousePosition().y;
}

glm::vec2 Input::GetMouseDelta() {
    if (!s_Window) return glm::vec2(0.0f);
    
    glm::vec2 currentPos = GetMousePosition();
    
    if (s_FirstMouse) {
        s_LastMousePosition = currentPos;
        s_FirstMouse = false;
    }
    
    s_MouseDelta = currentPos - s_LastMousePosition;
    s_LastMousePosition = currentPos;
    
    return s_MouseDelta;
}

void Input::SetMouseDelta(float x, float y) {
    s_MouseDelta = glm::vec2(x, y);
}

void Input::ResetMouseDelta() {
    s_MouseDelta = glm::vec2(0.0f);
    s_FirstMouse = true;
}

void Input::SetCursorEnabled(bool enabled) {
    if (!s_Window) return;
    
    if (enabled) {
        glfwSetInputMode(s_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(s_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

