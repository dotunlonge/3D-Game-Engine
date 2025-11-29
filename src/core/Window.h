#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <functional>

struct WindowProps {
    int Width = 1280;
    int Height = 720;
    std::string Title = "CaffeineForest";
};

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    void OnUpdate();
    void SwapBuffers();
    bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }

    int GetWidth() const { return m_Data.Width; }
    int GetHeight() const { return m_Data.Height; }

    GLFWwindow* GetNativeWindow() const { return m_Window; }

    using EventCallbackFn = std::function<void()>;
    void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

private:
    GLFWwindow* m_Window;
    WindowProps m_Data;
    EventCallbackFn m_EventCallback;
};

