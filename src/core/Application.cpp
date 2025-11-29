#include "Application.h"
#include "../core/Logger.h"
#include "../core/Layer.h"
#include "../renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <algorithm>

Application* Application::s_Instance = nullptr;

Application::Application() {
    if (s_Instance) {
        LOG_ERROR("Application already exists!");
        return;
    }
    s_Instance = this;

    m_Window = std::make_unique<Window>(1280, 720, "CaffeineForest - Proof of Suffering");
    m_ImGuiLayer = std::make_unique<ImGuiLayer>();
    m_ImGuiLayer->OnAttach();
    
    // Set viewport
    Renderer::SetViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
    Renderer::SetClearColor(glm::vec4(0.13f, 0.16f, 0.18f, 1.0f));
}

Application::~Application() {
    m_ImGuiLayer->OnDetach();
    s_Instance = nullptr;
}

void Application::Run() {
    while (m_Running && !m_Window->ShouldClose()) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - m_LastFrameTime;
        m_LastFrameTime = currentTime;

        // Update window (poll events)
        m_Window->OnUpdate();
        
        // Update viewport if window resized
        int width = m_Window->GetWidth();
        int height = m_Window->GetHeight();
        Renderer::SetViewport(0, 0, width, height);
        
        // Update engine systems
        Update(deltaTime);
        
        // Render frame
        Render();
    }
}

void Application::Update(float deltaTime) {
    // Handle input
    if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
        Shutdown();
    }
    
    // Engine update logic (scene, physics, animation, etc.)
}

void Application::Render() {
    // Clear screen
    Renderer::Clear();
    
    // Render layers
    for (auto& layer : m_LayerStack) {
        if (layer) {
            layer->OnRender();
        }
    }
    
    // Begin ImGui frame
    m_ImGuiLayer->Begin();
    
    // Render ImGui
    m_ImGuiLayer->End();
    
    // Swap buffers
    m_Window->SwapBuffers();
}

void Application::PushLayer(std::shared_ptr<class Layer> layer) {
    m_LayerStack.push_back(layer);
    layer->OnAttach();
}

void Application::PopLayer(std::shared_ptr<class Layer> layer) {
    auto it = std::find(m_LayerStack.begin(), m_LayerStack.end(), layer);
    if (it != m_LayerStack.end()) {
        layer->OnDetach();
        m_LayerStack.erase(it);
    }
}

void Application::Shutdown() {
    m_Running = false;
}

