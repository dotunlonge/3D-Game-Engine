#include "GameLayer.h"
#include "../core/Input.h"
#include "../core/Application.h"
#include "../utils/ResourceManager.h"
#include "../renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

GameLayer::GameLayer() : Layer("GameLayer") {
}

GameLayer::~GameLayer() {
}

void GameLayer::OnAttach() {
    // Create scene
    m_Scene = std::make_unique<Scene>();
    
    // Create camera
    Application& app = Application::Get();
    float aspect = (float)app.GetWindow().GetWidth() / (float)app.GetWindow().GetHeight();
    m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 5.0f), 45.0f, aspect);
    
    // Load default shader
    m_DefaultShader = ResourceManager::LoadShader("default", 
        "assets/shaders/default.vert", 
        "assets/shaders/default.frag");
    
    // Create some test entities
    // Entity* entity = m_Scene->CreateEntity();
    // entity->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
}

void GameLayer::OnDetach() {
    m_Scene.reset();
    m_Camera.reset();
    m_DefaultShader.reset();
}

void GameLayer::OnUpdate(float deltaTime) {
    // Update camera with input
    if (Input::IsKeyPressed(GLFW_KEY_W)) {
        m_Camera->ProcessKeyboard(0, deltaTime); // Forward
    }
    if (Input::IsKeyPressed(GLFW_KEY_S)) {
        m_Camera->ProcessKeyboard(1, deltaTime); // Backward
    }
    if (Input::IsKeyPressed(GLFW_KEY_A)) {
        m_Camera->ProcessKeyboard(2, deltaTime); // Left
    }
    if (Input::IsKeyPressed(GLFW_KEY_D)) {
        m_Camera->ProcessKeyboard(3, deltaTime); // Right
    }
    
    // Update scene
    m_Scene->Update(deltaTime);
}

void GameLayer::OnRender() {
    if (!m_DefaultShader || !m_Camera) return;
    
    // Use shader
    m_DefaultShader->Bind();
    
    // Set view-projection matrix
    m_DefaultShader->SetUniformMat4("uViewProj", m_Camera->GetViewProjectionMatrix());
    
    // Render scene entities
    // For now, this is a placeholder - add model rendering when models are loaded
    m_Scene->Render();
    
    m_DefaultShader->Unbind();
}

