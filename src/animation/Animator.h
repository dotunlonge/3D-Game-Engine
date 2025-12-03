#pragma once

#include "Skeleton.h"
#include "AnimationClip.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <vector>

struct AnimationLayer {
    std::shared_ptr<AnimationClip> clip;
    float currentTime = 0.0f;
    float weight = 1.0f;
    bool looping = true;
};

class Animator {
public:
    Animator(Skeleton* skeleton);
    ~Animator();

    void UpdateAnimation(float deltaTime);
    
    // Single animation playback
    void PlayAnimation(std::shared_ptr<AnimationClip> clip);
    void StopAnimation();
    
    // Blending support
    int AddAnimationLayer(std::shared_ptr<AnimationClip> clip, float weight = 1.0f);
    void RemoveAnimationLayer(int layerID);
    void SetLayerWeight(int layerID, float weight);
    void SetLayerLooping(int layerID, bool looping);
    void CrossFadeTo(std::shared_ptr<AnimationClip> clip, float fadeTime = 0.3f);

    std::vector<glm::mat4> GetFinalBoneMatrices() const { return m_FinalBoneMatrices; }

private:
    void CalculateBoneTransform(const BoneAnimation* anim, float currentTime, int boneID, const glm::mat4& parentTransform);
    void BlendBoneTransforms(int boneID, const glm::mat4& parentTransform);
    glm::vec3 InterpolatePosition(BoneAnimation* anim, float time);
    glm::quat InterpolateRotation(BoneAnimation* anim, float time);
    glm::vec3 InterpolateScale(BoneAnimation* anim, float time);
    
    glm::mat4 CalculateBoneTransformForAnimation(AnimationLayer& layer, int boneID, const glm::mat4& parentTransform);

    Skeleton* m_Skeleton;
    std::shared_ptr<AnimationClip> m_CurrentAnimation; // Legacy single animation
    float m_CurrentTime = 0.0f;
    std::vector<glm::mat4> m_FinalBoneMatrices;
    
    // Blending system
    std::vector<AnimationLayer> m_AnimationLayers;
    int m_NextLayerID = 0;
    bool m_UseBlending = false;
    
    // Crossfade
    AnimationLayer* m_CrossfadeFrom = nullptr;
    AnimationLayer* m_CrossfadeTo = nullptr;
    float m_CrossfadeTime = 0.0f;
    float m_CrossfadeDuration = 0.3f;
};

