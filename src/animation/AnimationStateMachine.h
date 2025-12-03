#pragma once

#include "AnimationState.h"
#include "Animator.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

// Animation state machine for character controllers
class AnimationStateMachine {
public:
    AnimationStateMachine(Animator* animator);
    ~AnimationStateMachine();

    // State management
    void AddState(const std::string& name, std::shared_ptr<AnimationClip> clip, bool looping = true);
    void SetDefaultState(const std::string& name);
    
    // Transition management
    void AddTransition(const std::string& from, const std::string& to, 
                     TransitionCondition condition, float fadeTime = 0.3f);
    
    // State machine control
    void Update(float deltaTime);
    void ForceState(const std::string& stateName, float fadeTime = 0.0f);
    std::string GetCurrentState() const { return m_CurrentStateName; }
    
    // State queries
    bool HasState(const std::string& name) const;
    float GetStateTime(const std::string& name) const;

private:
    void EvaluateTransitions();
    void TransitionTo(const std::string& stateName, float fadeTime);
    
    Animator* m_Animator;
    std::unordered_map<std::string, std::unique_ptr<AnimationState>> m_States;
    std::vector<StateTransition> m_Transitions;
    std::string m_CurrentStateName;
    std::string m_DefaultStateName;
    float m_CurrentStateTime = 0.0f;
};

