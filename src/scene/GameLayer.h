#pragma once

#include "../scene/Scene.h"
#include "../scene/Camera.h"
#include "../renderer/Shader.h"
#include "../renderer/Model.h"
#include "../renderer/Mesh.h"
#include "../renderer/Primitives.h"
#include "../renderer/AnimatedModel.h"
#include "../core/Layer.h"
#include <memory>

// Example game layer - demonstrates engine usage
class GameLayer : public Layer {
public:
    GameLayer();
    ~GameLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;

private:
    std::unique_ptr<Scene> m_Scene;
    std::unique_ptr<Camera> m_Camera;
    std::shared_ptr<Shader> m_DefaultShader;
    std::shared_ptr<Shader> m_SkinnedShader;
    std::unique_ptr<Mesh> m_TestCube;
    glm::mat4 m_CubeTransform;
    float m_Rotation = 0.0f;
    
    // Animated model example (uncomment when you have an animated model)
    // std::unique_ptr<AnimatedModel> m_AnimatedCharacter;
    // glm::mat4 m_CharacterTransform;
};

