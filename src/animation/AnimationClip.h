#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Keyframe {
    float Time;
    glm::vec3 Position;
    glm::quat Rotation;
    glm::vec3 Scale;
};

struct BoneAnimation {
    std::string BoneName;
    std::vector<Keyframe> PositionKeys;
    std::vector<Keyframe> RotationKeys;
    std::vector<Keyframe> ScaleKeys;
};

class AnimationClip {
public:
    AnimationClip(const std::string& name, float duration, float ticksPerSecond);
    ~AnimationClip();

    void AddBoneAnimation(const BoneAnimation& anim);
    BoneAnimation* GetBoneAnimation(const std::string& boneName);

    std::string GetName() const { return m_Name; }
    float GetDuration() const { return m_Duration; }
    float GetTicksPerSecond() const { return m_TicksPerSecond; }

private:
    std::string m_Name;
    float m_Duration;
    float m_TicksPerSecond;
    std::vector<BoneAnimation> m_BoneAnimations;
};

