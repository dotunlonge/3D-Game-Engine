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
    if (!m_CurrentAnimation || !m_Skeleton) return;

    float ticksPerSecond = m_CurrentAnimation->GetTicksPerSecond() != 0 ? 
                           m_CurrentAnimation->GetTicksPerSecond() : 25.0f;
    m_CurrentTime += ticksPerSecond * deltaTime;
    m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());

    // Start from root bone (ID 0)
    if (m_Skeleton->GetBoneCount() > 0) {
        CalculateBoneTransform(nullptr, m_CurrentTime, 0, glm::mat4(1.0f));
    }
}

void Animator::PlayAnimation(std::shared_ptr<AnimationClip> clip) {
    m_CurrentAnimation = clip;
    m_CurrentTime = 0.0f;
}

void Animator::StopAnimation() {
    m_CurrentAnimation = nullptr;
    m_CurrentTime = 0.0f;
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

