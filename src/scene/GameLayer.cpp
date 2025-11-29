#include "GameLayer.h"
#include "../core/Input.h"
#include "../core/Application.h"
#include "../utils/ResourceManager.h"
#include "../renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

GameLayer::GameLayer() : Layer("GameLayer"), m_CubeTransform(1.0f) {
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
    
    // Disable cursor for FPS-style camera
    Input::SetCursorEnabled(false);
    Input::ResetMouseDelta();
    
    // Load default shader
    m_DefaultShader = ResourceManager::LoadShader("default", 
        "assets/shaders/default.vert", 
        "assets/shaders/default.frag");
    
    // Create test cube
    m_TestCube = std::make_unique<Mesh>(Primitives::CreateCube());
    m_CubeTransform = glm::mat4(1.0f);
    
    // Create some test entities
    // Entity* entity = m_Scene->CreateEntity();
    // entity->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
}

void GameLayer::OnDetach() {
    Input::SetCursorEnabled(true);
    m_Scene.reset();
    m_Camera.reset();
    m_DefaultShader.reset();
    m_TestCube.reset();
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
    
    // Mouse look
    glm::vec2 mouseDelta = Input::GetMouseDelta();
    if (glm::length(mouseDelta) > 0.0f) {
        m_Camera->ProcessMouseMovement(mouseDelta.x, mouseDelta.y);
    }
    
    // Update camera aspect ratio if window resized
    Application& app = Application::Get();
    float aspect = (float)app.GetWindow().GetWidth() / (float)app.GetWindow().GetHeight();
    m_Camera->SetAspect(aspect);
    
    // Update scene
    m_Scene->Update(deltaTime);
}

void GameLayer::OnRender() {
    if (!m_DefaultShader || !m_Camera || !m_TestCube) return;
    
    // Use shader
    m_DefaultShader->Bind();
    
    // Set view-projection matrix
    m_DefaultShader->SetUniformMat4("uViewProj", m_Camera->GetViewProjectionMatrix());
    
    // Update cube rotation
    m_Rotation += 0.01f;
    m_CubeTransform = glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Render test cube
    m_DefaultShader->SetUniformMat4("uModel", m_CubeTransform);
    m_TestCube->Draw(*m_DefaultShader);
    
    // Render scene entities
    m_Scene->Render();
    
    m_DefaultShader->Unbind();
}

