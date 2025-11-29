#include "Application.h"
#include "../core/Logger.h"
#include <GLFW/glfw3.h>

Application* Application::s_Instance = nullptr;

Application::Application() {
    if (s_Instance) {
        LOG_ERROR("Application already exists!");
        return;
    }
    s_Instance = this;

    m_Window = std::make_unique<Window>(1280, 720, "CaffeineForest");
    m_ImGuiLayer = std::make_unique<ImGuiLayer>();
    m_ImGuiLayer->OnAttach();
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

        m_Window->OnUpdate();
        Update(deltaTime);
        Render();
    }
}

void Application::Update(float deltaTime) {
    // Engine update logic
}

void Application::Render() {
    m_ImGuiLayer->Begin();
    // Render calls here
    m_ImGuiLayer->End();
    m_Window->SwapBuffers();
}

void Application::Shutdown() {
    m_Running = false;
}

