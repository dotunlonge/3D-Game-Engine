#pragma once

#include "Skeleton.h"
#include "AnimationClip.h"
#include <glm/glm.hpp>
#include <memory>

class Animator {
public:
    Animator(Skeleton* skeleton);
    ~Animator();

    void UpdateAnimation(float deltaTime);
    void PlayAnimation(std::shared_ptr<AnimationClip> clip);
    void StopAnimation();

    std::vector<glm::mat4> GetFinalBoneMatrices() const { return m_FinalBoneMatrices; }

private:
    void CalculateBoneTransform(const BoneAnimation* anim, float currentTime, int boneID, const glm::mat4& parentTransform);

    Skeleton* m_Skeleton;
    std::shared_ptr<AnimationClip> m_CurrentAnimation;
    float m_CurrentTime = 0.0f;
    std::vector<glm::mat4> m_FinalBoneMatrices;
};

