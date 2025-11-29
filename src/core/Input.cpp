#include "Input.h"

GLFWwindow* Input::s_Window = nullptr;

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

