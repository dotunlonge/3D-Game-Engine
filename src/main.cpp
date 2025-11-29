// src/main.cpp
#include "core/Application.h"
#include "core/Logger.h"
#include "renderer/Renderer.h"
#include "scene/GameLayer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

// Entry point - uses Application architecture
int main() {
    LOG_INFO("Starting CaffeineForest Engine...");

    // Initialize GLFW error callback
    glfwSetErrorCallback([](int error, const char* description) {
        LOG_ERROR("GLFW Error " + std::to_string(error) + ": " + description);
    });

    // Create and run application
    Application app;
    
    // Initialize renderer
    Renderer::Init();
    
    // Set up input system
    Input::SetWindow(app.GetWindow().GetNativeWindow());
    
    LOG_INFO("Engine initialized. Starting main loop...");
    
    // Push game layer (example usage - uncomment when ready)
    // auto gameLayer = std::make_shared<GameLayer>();
    // app.PushLayer(gameLayer);
    
    // Run main loop
    app.Run();
    
    // Cleanup
    Renderer::Shutdown();
    LOG_INFO("Engine shutdown complete.");
    
    return 0;
}
