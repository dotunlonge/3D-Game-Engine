#include "AnimationStateMachine.h"
#include "../core/Logger.h"
#include <algorithm>

AnimationStateMachine::AnimationStateMachine(Animator* animator) 
    : m_Animator(animator) {
    if (!animator) {
        LOG_ERROR("AnimationStateMachine: Animator is null!");
    }
}

AnimationStateMachine::~AnimationStateMachine() {
}

void AnimationStateMachine::AddState(const std::string& name, std::shared_ptr<AnimationClip> clip, bool looping) {
    if (m_States.find(name) != m_States.end()) {
        LOG_WARN("AnimationStateMachine: State '" + name + "' already exists, replacing");
    }
    
    auto state = std::make_unique<AnimationState>(name, clip);
    state->SetLooping(looping);
    m_States[name] = std::move(state);
    
    // Set as default if first state
    if (m_States.size() == 1 && m_DefaultStateName.empty()) {
        m_DefaultStateName = name;
        m_CurrentStateName = name;
    }
}

void AnimationStateMachine::SetDefaultState(const std::string& name) {
    if (m_States.find(name) == m_States.end()) {
        LOG_ERROR("AnimationStateMachine: Cannot set default state '" + name + "' - state not found");
        return;
    }
    m_DefaultStateName = name;
    if (m_CurrentStateName.empty()) {
        m_CurrentStateName = name;
    }
}

void AnimationStateMachine::AddTransition(const std::string& from, const std::string& to, 
                                         TransitionCondition condition, float fadeTime) {
    if (m_States.find(from) == m_States.end()) {
        LOG_ERROR("AnimationStateMachine: Transition from state '" + from + "' not found");
        return;
    }
    if (m_States.find(to) == m_States.end()) {
        LOG_ERROR("AnimationStateMachine: Transition to state '" + to + "' not found");
        return;
    }
    
    StateTransition transition;
    transition.fromState = from;
    transition.toState = to;
    transition.condition = condition;
    transition.fadeTime = fadeTime;
    m_Transitions.push_back(transition);
}

void AnimationStateMachine::Update(float deltaTime) {
    if (!m_Animator || m_CurrentStateName.empty()) return;
    
    auto it = m_States.find(m_CurrentStateName);
    if (it == m_States.end()) {
        LOG_ERROR("AnimationStateMachine: Current state '" + m_CurrentStateName + "' not found");
        return;
    }
    
    // Evaluate transitions
    EvaluateTransitions();
    
    // Update state time
    m_CurrentStateTime += deltaTime;
}

void AnimationStateMachine::EvaluateTransitions() {
    if (m_CurrentStateName.empty()) return;
    
    // Check all transitions from current state
    for (const auto& transition : m_Transitions) {
        if (transition.fromState == m_CurrentStateName) {
            // Check condition
            if (transition.condition && transition.condition()) {
                TransitionTo(transition.toState, transition.fadeTime);
                break; // Only take first valid transition
            }
        }
    }
}

void AnimationStateMachine::TransitionTo(const std::string& stateName, float fadeTime) {
    if (m_States.find(stateName) == m_States.end()) {
        LOG_ERROR("AnimationStateMachine: Cannot transition to state '" + stateName + "' - not found");
        return;
    }
    
    auto* state = m_States[stateName].get();
    if (!state || !state->GetClip()) {
        LOG_ERROR("AnimationStateMachine: State '" + stateName + "' has no clip");
        return;
    }
    
    if (fadeTime > 0.0f) {
        m_Animator->CrossFadeTo(state->GetClip(), fadeTime);
    } else {
        m_Animator->PlayAnimation(state->GetClip());
    }
    
    m_CurrentStateName = stateName;
    m_CurrentStateTime = 0.0f;
}

void AnimationStateMachine::ForceState(const std::string& stateName, float fadeTime) {
    TransitionTo(stateName, fadeTime);
}

bool AnimationStateMachine::HasState(const std::string& name) const {
    return m_States.find(name) != m_States.end();
}

float AnimationStateMachine::GetStateTime(const std::string& name) const {
    if (name == m_CurrentStateName) {
        return m_CurrentStateTime;
    }
    return 0.0f;
}

