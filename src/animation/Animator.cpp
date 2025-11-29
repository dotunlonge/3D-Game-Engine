#include "Animator.h"
#include <glm/gtx/quaternion.hpp>
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

    m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * deltaTime;
    m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());

    CalculateBoneTransform(nullptr, m_CurrentTime, 0, glm::mat4(1.0f));
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
    // Simplified - full implementation would:
    // 1. Find keyframes for current time
    // 2. Interpolate position, rotation, scale
    // 3. Build local transform matrix
    // 4. Combine with parent
    // 5. Multiply by offset matrix
    // 6. Store in m_FinalBoneMatrices
    // 7. Recursively process children

    glm::mat4 localTransform = glm::mat4(1.0f);
    if (anim) {
        // Interpolate keyframes here
    }

    glm::mat4 globalTransform = parentTransform * localTransform;
    Bone* bone = m_Skeleton->GetBone(boneID);
    if (bone) {
        m_FinalBoneMatrices[boneID] = globalTransform * bone->OffsetMatrix;
    }
}

