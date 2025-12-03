#include "AnimatedModel.h"
#include "../core/Logger.h"
#include "../animation/AnimationClip.h"
#include <glm/gtc/matrix_transform.hpp>

AnimatedModel::AnimatedModel(const std::string& modelPath) {
    // Load model
    m_Model = std::make_unique<Model>(modelPath);
    
    // Create animator if skeleton exists
    auto* skeleton = m_Model->GetSkeleton();
    if (skeleton && skeleton->GetBoneCount() > 0) {
        m_Animator = std::make_unique<Animator>(skeleton);
        m_GPUAnimator = std::make_unique<GPUAnimator>();
        m_UseSkinnedShader = true;
        LOG_INFO("AnimatedModel: Skeleton loaded with " + std::to_string(skeleton->GetBoneCount()) + " bones");
    } else {
        LOG_WARN("AnimatedModel: No skeleton found in model");
    }
    
    // Get animations from model
    m_Animations = m_Model->GetAnimations();
    if (!m_Animations.empty()) {
        LOG_INFO("AnimatedModel: Loaded " + std::to_string(m_Animations.size()) + " animation(s)");
    }
}

AnimatedModel::~AnimatedModel() {
}

void AnimatedModel::Update(float deltaTime) {
    if (m_Animator) {
        m_Animator->UpdateAnimation(deltaTime);
        
        // Upload bone matrices to GPU
        if (m_GPUAnimator) {
            auto boneMatrices = m_Animator->GetFinalBoneMatrices();
            m_GPUAnimator->UploadBoneMatrices(boneMatrices);
        }
    }
}

void AnimatedModel::Draw(Shader& shader, const glm::mat4& transform) {
    if (!m_Model) return;
    
    // Bind bone matrices if animating
    if (m_UseSkinnedShader && m_GPUAnimator && m_Animator && IsAnimating()) {
        m_GPUAnimator->Bind();
    }
    
    // Set model matrix
    shader.SetUniformMat4("uModel", transform);
    
    // Draw model
    m_Model->Draw(shader);
    
    // Unbind SSBO
    if (m_UseSkinnedShader && m_GPUAnimator) {
        m_GPUAnimator->Unbind();
    }
}

void AnimatedModel::PlayAnimation(int index) {
    if (!m_Animator) {
        LOG_WARN("AnimatedModel: Cannot play animation - no animator");
        return;
    }
    
    if (index < 0 || index >= (int)m_Animations.size()) {
        LOG_WARN("AnimatedModel: Animation index out of range: " + std::to_string(index));
        return;
    }
    
    m_Animator->PlayAnimation(m_Animations[index]);
    m_CurrentAnimationIndex = index;
    LOG_INFO("AnimatedModel: Playing animation " + std::to_string(index));
}

void AnimatedModel::PlayAnimation(const std::string& name) {
    if (!m_Animator) {
        LOG_WARN("AnimatedModel: Cannot play animation - no animator");
        return;
    }
    
    for (size_t i = 0; i < m_Animations.size(); i++) {
        if (m_Animations[i]->GetName() == name) {
            PlayAnimation((int)i);
            return;
        }
    }
    
    LOG_WARN("AnimatedModel: Animation not found: " + name);
}

void AnimatedModel::StopAnimation() {
    if (m_Animator) {
        m_Animator->StopAnimation();
        m_CurrentAnimationIndex = -1;
    }
}

std::vector<std::shared_ptr<AnimationClip>> AnimatedModel::GetAnimations() const {
    return m_Animations;
}

void AnimatedModel::CrossFadeTo(int animationIndex, float fadeTime) {
    if (!m_Animator || animationIndex < 0 || animationIndex >= (int)m_Animations.size()) {
        return;
    }
    m_Animator->CrossFadeTo(m_Animations[animationIndex], fadeTime);
    m_CurrentAnimationIndex = animationIndex;
}

void AnimatedModel::CrossFadeTo(const std::string& name, float fadeTime) {
    for (size_t i = 0; i < m_Animations.size(); i++) {
        if (m_Animations[i]->GetName() == name) {
            CrossFadeTo((int)i, fadeTime);
            return;
        }
    }
}

int AnimatedModel::AddAnimationLayer(int animationIndex, float weight) {
    if (!m_Animator || animationIndex < 0 || animationIndex >= (int)m_Animations.size()) {
        return -1;
    }
    return m_Animator->AddAnimationLayer(m_Animations[animationIndex], weight);
}

void AnimatedModel::SetLayerWeight(int layerID, float weight) {
    if (m_Animator) {
        m_Animator->SetLayerWeight(layerID, weight);
    }
}

