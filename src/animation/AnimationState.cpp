#include "AnimationState.h"

AnimationState::AnimationState(const std::string& name, std::shared_ptr<AnimationClip> clip)
    : m_Name(name), m_Clip(clip) {
}

AnimationState::~AnimationState() {
}

