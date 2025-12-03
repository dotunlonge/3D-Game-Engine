#pragma once

#include "AnimationClip.h"
#include <string>
#include <memory>
#include <functional>

// Represents a single animation state
class AnimationState {
public:
    AnimationState(const std::string& name, std::shared_ptr<AnimationClip> clip);
    ~AnimationState();

    std::string GetName() const { return m_Name; }
    std::shared_ptr<AnimationClip> GetClip() const { return m_Clip; }
    
    // State properties
    void SetLooping(bool looping) { m_Looping = looping; }
    bool IsLooping() const { return m_Looping; }
    
    void SetSpeed(float speed) { m_Speed = speed; }
    float GetSpeed() const { return m_Speed; }

private:
    std::string m_Name;
    std::shared_ptr<AnimationClip> m_Clip;
    bool m_Looping = true;
    float m_Speed = 1.0f;
};

// Transition condition (returns true when transition should occur)
using TransitionCondition = std::function<bool()>;

// Represents a transition between states
struct StateTransition {
    std::string fromState;
    std::string toState;
    TransitionCondition condition;
    float fadeTime = 0.3f;
    bool canInterrupt = true;
};

