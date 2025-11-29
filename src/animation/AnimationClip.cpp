#include "AnimationClip.h"

AnimationClip::AnimationClip(const std::string& name, float duration, float ticksPerSecond)
    : m_Name(name), m_Duration(duration), m_TicksPerSecond(ticksPerSecond) {
}

AnimationClip::~AnimationClip() {
}

void AnimationClip::AddBoneAnimation(const BoneAnimation& anim) {
    m_BoneAnimations.push_back(anim);
}

BoneAnimation* AnimationClip::GetBoneAnimation(const std::string& boneName) {
    for (auto& anim : m_BoneAnimations) {
        if (anim.BoneName == boneName) {
            return &anim;
        }
    }
    return nullptr;
}

