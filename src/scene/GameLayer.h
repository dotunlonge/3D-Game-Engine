#pragma once

#include "../scene/Scene.h"
#include "../scene/Camera.h"
#include "../renderer/Shader.h"
#include "../renderer/Model.h"
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
    // std::shared_ptr<Model> m_TestModel; // Uncomment when you have a model
};

