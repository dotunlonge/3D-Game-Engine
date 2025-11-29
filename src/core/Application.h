#pragma once

#include "Window.h"
#include "Input.h"
#include "../debug/ImGuiLayer.h"
#include <memory>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Forward declaration
class Layer;

class Application {
public:
    Application();
    ~Application();

    void Run();
    void Shutdown();

    static Application& Get() { return *s_Instance; }
    Window& GetWindow() { return *m_Window; }

    // Layer management (for future expansion)
    void PushLayer(std::shared_ptr<class Layer> layer);
    void PopLayer(std::shared_ptr<class Layer> layer);

private:
    void Update(float deltaTime);
    void Render();

    std::unique_ptr<Window> m_Window;
    std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
    std::vector<std::shared_ptr<class Layer>> m_LayerStack;
    bool m_Running = true;
    float m_LastFrameTime = 0.0f;

    static Application* s_Instance;
};

