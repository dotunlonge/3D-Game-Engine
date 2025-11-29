#pragma once

#include "Model.h"
#include "Shader.h"
#include "../animation/Animator.h"
#include "../animation/AnimationClip.h"
#include "GPUAnimator.h"
#include <memory>
#include <vector>

// Wrapper class for animated models that handles animation and rendering
class AnimatedModel {
public:
    AnimatedModel(const std::string& modelPath);
    ~AnimatedModel();

    void Update(float deltaTime);
    void Draw(Shader& shader, const glm::mat4& transform = glm::mat4(1.0f));
    
    // Animation control
    void PlayAnimation(int index);
    void PlayAnimation(const std::string& name);
    void StopAnimation();
    bool IsAnimating() const { return m_Animator && m_CurrentAnimationIndex >= 0; }
    
    // Getters
    Model* GetModel() { return m_Model.get(); }
    Animator* GetAnimator() { return m_Animator.get(); }
    std::vector<std::shared_ptr<class AnimationClip>> GetAnimations() const;

private:
    std::unique_ptr<Model> m_Model;
    std::unique_ptr<Animator> m_Animator;
    std::unique_ptr<GPUAnimator> m_GPUAnimator;
    std::vector<std::shared_ptr<class AnimationClip>> m_Animations;
    int m_CurrentAnimationIndex = -1;
    bool m_UseSkinnedShader = false;
};

