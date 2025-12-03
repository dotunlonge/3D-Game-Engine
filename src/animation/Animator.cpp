#include "Animator.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

Animator::Animator(Skeleton* skeleton) : m_Skeleton(skeleton) {
    if (skeleton) {
        m_FinalBoneMatrices.resize(skeleton->GetBoneCount(), glm::mat4(1.0f));
    }
}

Animator::~Animator() {
}

void Animator::UpdateAnimation(float deltaTime) {
    if (!m_Skeleton || m_Skeleton->GetBoneCount() == 0) return;
    
    // Update crossfade
    if (m_CrossfadeFrom && m_CrossfadeTo) {
        m_CrossfadeTime += deltaTime;
        float fadeProgress = m_CrossfadeTime / m_CrossfadeDuration;
        
        if (fadeProgress >= 1.0f) {
            // Crossfade complete
            m_CrossfadeFrom->weight = 0.0f;
            m_CrossfadeTo->weight = 1.0f;
            m_CrossfadeFrom = nullptr;
            m_CrossfadeTo = nullptr;
            m_CrossfadeTime = 0.0f;
        } else {
            // Interpolate weights
            m_CrossfadeFrom->weight = 1.0f - fadeProgress;
            m_CrossfadeTo->weight = fadeProgress;
        }
    }
    
    // Update animation layers
    if (m_UseBlending && !m_AnimationLayers.empty()) {
        for (auto& layer : m_AnimationLayers) {
            if (layer.clip && layer.weight > 0.0f) {
                float ticksPerSecond = layer.clip->GetTicksPerSecond() != 0 ? 
                                      layer.clip->GetTicksPerSecond() : 25.0f;
                layer.currentTime += ticksPerSecond * deltaTime;
                
                if (layer.looping) {
                    layer.currentTime = fmod(layer.currentTime, layer.clip->GetDuration());
                } else {
                    layer.currentTime = std::min(layer.currentTime, layer.clip->GetDuration());
                }
            }
        }
        
        // Blend bone transforms
        if (m_Skeleton->GetBoneCount() > 0) {
            BlendBoneTransforms(0, glm::mat4(1.0f));
        }
    } else if (m_CurrentAnimation) {
        // Legacy single animation mode
        float ticksPerSecond = m_CurrentAnimation->GetTicksPerSecond() != 0 ? 
                              m_CurrentAnimation->GetTicksPerSecond() : 25.0f;
        m_CurrentTime += ticksPerSecond * deltaTime;
        m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());

        if (m_Skeleton->GetBoneCount() > 0) {
            CalculateBoneTransform(nullptr, m_CurrentTime, 0, glm::mat4(1.0f));
        }
    }
}

void Animator::PlayAnimation(std::shared_ptr<AnimationClip> clip) {
    m_CurrentAnimation = clip;
    m_CurrentTime = 0.0f;
}

void Animator::StopAnimation() {
    m_CurrentAnimation = nullptr;
    m_CurrentTime = 0.0f;
    m_UseBlending = false;
    m_AnimationLayers.clear();
}

int Animator::AddAnimationLayer(std::shared_ptr<AnimationClip> clip, float weight) {
    AnimationLayer layer;
    layer.clip = clip;
    layer.weight = weight;
    layer.currentTime = 0.0f;
    layer.looping = true;
    
    int layerID = m_NextLayerID++;
    m_AnimationLayers.push_back(layer);
    m_UseBlending = true;
    
    return layerID;
}

void Animator::RemoveAnimationLayer(int layerID) {
    // Note: Simplified implementation - layerID is index
    // In production, use a map to track layer IDs properly
    if (layerID >= 0 && layerID < (int)m_AnimationLayers.size()) {
        m_AnimationLayers.erase(m_AnimationLayers.begin() + layerID);
    }
    
    if (m_AnimationLayers.empty()) {
        m_UseBlending = false;
    }
}

void Animator::SetLayerWeight(int layerID, float weight) {
    if (layerID >= 0 && layerID < (int)m_AnimationLayers.size()) {
        m_AnimationLayers[layerID].weight = glm::clamp(weight, 0.0f, 1.0f);
    }
}

void Animator::SetLayerLooping(int layerID, bool looping) {
    if (layerID >= 0 && layerID < (int)m_AnimationLayers.size()) {
        m_AnimationLayers[layerID].looping = looping;
    }
}

void Animator::CrossFadeTo(std::shared_ptr<AnimationClip> clip, float fadeTime) {
    m_CrossfadeDuration = fadeTime;
    m_CrossfadeTime = 0.0f;
    
    // Find current playing layer or create from current animation
    if (!m_AnimationLayers.empty() && m_AnimationLayers[0].weight > 0.0f) {
        m_CrossfadeFrom = &m_AnimationLayers[0];
    } else if (m_CurrentAnimation) {
        // Convert current animation to layer
        AnimationLayer layer;
        layer.clip = m_CurrentAnimation;
        layer.currentTime = m_CurrentTime;
        layer.weight = 1.0f;
        m_AnimationLayers.insert(m_AnimationLayers.begin(), layer);
        m_CrossfadeFrom = &m_AnimationLayers[0];
        m_UseBlending = true;
    }
    
    // Add new animation as crossfade target
    AnimationLayer newLayer;
    newLayer.clip = clip;
    newLayer.currentTime = 0.0f;
    newLayer.weight = 0.0f;
    m_AnimationLayers.push_back(newLayer);
    m_CrossfadeTo = &m_AnimationLayers.back();
}

void Animator::BlendBoneTransforms(int boneID, const glm::mat4& parentTransform) {
    if (!m_Skeleton) return;
    
    Bone* bone = m_Skeleton->GetBone(boneID);
    if (!bone) return;
    
    glm::mat4 blendedTransform = glm::mat4(0.0f);
    float totalWeight = 0.0f;
    
    // Blend transforms from all active layers
    for (auto& layer : m_AnimationLayers) {
        if (layer.clip && layer.weight > 0.0f) {
            glm::mat4 layerTransform = CalculateBoneTransformForAnimation(layer, boneID, parentTransform);
            blendedTransform += layerTransform * layer.weight;
            totalWeight += layer.weight;
        }
    }
    
    // Normalize if weights don't sum to 1.0
    if (totalWeight > 0.0f) {
        blendedTransform /= totalWeight;
    } else {
        blendedTransform = glm::mat4(1.0f);
    }
    
    m_FinalBoneMatrices[boneID] = blendedTransform * bone->OffsetMatrix;
    
    // Process children
    for (int childID : bone->Children) {
        BlendBoneTransforms(childID, blendedTransform);
    }
}

glm::mat4 Animator::CalculateBoneTransformForAnimation(AnimationLayer& layer, int boneID, const glm::mat4& parentTransform) {
    if (!m_Skeleton) return glm::mat4(1.0f);
    
    Bone* bone = m_Skeleton->GetBone(boneID);
    if (!bone) return glm::mat4(1.0f);
    
    glm::mat4 localTransform = glm::mat4(1.0f);
    BoneAnimation* boneAnim = layer.clip->GetBoneAnimation(bone->Name);
    
    if (boneAnim) {
        glm::vec3 position = InterpolatePosition(boneAnim, layer.currentTime);
        glm::quat rotation = InterpolateRotation(boneAnim, layer.currentTime);
        glm::vec3 scale = InterpolateScale(boneAnim, layer.currentTime);
        
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMat = glm::mat4_cast(rotation);
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
        
        localTransform = translation * rotationMat * scaleMat;
    }
    
    return parentTransform * localTransform;
}

void Animator::CalculateBoneTransform(const BoneAnimation* anim, float currentTime, int boneID, const glm::mat4& parentTransform) {
    if (!m_CurrentAnimation || !m_Skeleton) return;
    
    glm::mat4 localTransform = glm::mat4(1.0f);
    
    // Find bone animation data
    Bone* bone = m_Skeleton->GetBone(boneID);
    if (!bone) return;
    
    BoneAnimation* boneAnim = m_CurrentAnimation->GetBoneAnimation(bone->Name);
    
    if (boneAnim) {
        // Interpolate position
        glm::vec3 position = InterpolatePosition(boneAnim, currentTime);
        
        // Interpolate rotation
        glm::quat rotation = InterpolateRotation(boneAnim, currentTime);
        
        // Interpolate scale
        glm::vec3 scale = InterpolateScale(boneAnim, currentTime);
        
        // Build transformation matrix
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMat = glm::mat4_cast(rotation);
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
        
        localTransform = translation * rotationMat * scaleMat;
    }
    
    glm::mat4 globalTransform = parentTransform * localTransform;
    m_FinalBoneMatrices[boneID] = globalTransform * bone->OffsetMatrix;
    
    // Process children
    for (int childID : bone->Children) {
        CalculateBoneTransform(boneAnim, currentTime, childID, globalTransform);
    }
}

glm::vec3 Animator::InterpolatePosition(BoneAnimation* anim, float time) {
    if (!anim || anim->PositionKeys.empty()) return glm::vec3(0.0f);
    if (anim->PositionKeys.size() == 1) return anim->PositionKeys[0].Position;
    
    // Find keyframes
    int frameIndex = 0;
    for (int i = 0; i < anim->PositionKeys.size() - 1; i++) {
        if (time < anim->PositionKeys[i + 1].Time) {
            frameIndex = i;
            break;
        }
    }
    
    int nextFrame = (frameIndex + 1) % anim->PositionKeys.size();
    float deltaTime = anim->PositionKeys[nextFrame].Time - anim->PositionKeys[frameIndex].Time;
    float factor = (time - anim->PositionKeys[frameIndex].Time) / deltaTime;
    factor = glm::clamp(factor, 0.0f, 1.0f);
    
    return glm::mix(anim->PositionKeys[frameIndex].Position, 
                    anim->PositionKeys[nextFrame].Position, factor);
}

glm::quat Animator::InterpolateRotation(BoneAnimation* anim, float time) {
    if (!anim || anim->RotationKeys.empty()) return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    if (anim->RotationKeys.size() == 1) return anim->RotationKeys[0].Rotation;
    
    int frameIndex = 0;
    for (int i = 0; i < anim->RotationKeys.size() - 1; i++) {
        if (time < anim->RotationKeys[i + 1].Time) {
            frameIndex = i;
            break;
        }
    }
    
    int nextFrame = (frameIndex + 1) % anim->RotationKeys.size();
    float deltaTime = anim->RotationKeys[nextFrame].Time - anim->RotationKeys[frameIndex].Time;
    float factor = (time - anim->RotationKeys[frameIndex].Time) / deltaTime;
    factor = glm::clamp(factor, 0.0f, 1.0f);
    
    return glm::slerp(anim->RotationKeys[frameIndex].Rotation, 
                      anim->RotationKeys[nextFrame].Rotation, factor);
}

glm::vec3 Animator::InterpolateScale(BoneAnimation* anim, float time) {
    if (!anim || anim->ScaleKeys.empty()) return glm::vec3(1.0f);
    if (anim->ScaleKeys.size() == 1) return anim->ScaleKeys[0].Scale;
    
    int frameIndex = 0;
    for (int i = 0; i < anim->ScaleKeys.size() - 1; i++) {
        if (time < anim->ScaleKeys[i + 1].Time) {
            frameIndex = i;
            break;
        }
    }
    
    int nextFrame = (frameIndex + 1) % anim->ScaleKeys.size();
    float deltaTime = anim->ScaleKeys[nextFrame].Time - anim->ScaleKeys[frameIndex].Time;
    float factor = (time - anim->ScaleKeys[frameIndex].Time) / deltaTime;
    factor = glm::clamp(factor, 0.0f, 1.0f);
    
    return glm::mix(anim->ScaleKeys[frameIndex].Scale, 
                    anim->ScaleKeys[nextFrame].Scale, factor);
}

