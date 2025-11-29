#pragma once

#include "Window.h"
#include "Input.h"
#include "../debug/ImGuiLayer.h"
#include <memory>
#include <GLFW/glfw3.h>

class Application {
public:
    Application();
    ~Application();

    void Run();
    void Shutdown();

    static Application& Get() { return *s_Instance; }
    Window& GetWindow() { return *m_Window; }

private:
    void Update(float deltaTime);
    void Render();

    std::unique_ptr<Window> m_Window;
    std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
    bool m_Running = true;
    float m_LastFrameTime = 0.0f;

    static Application* s_Instance;
};

