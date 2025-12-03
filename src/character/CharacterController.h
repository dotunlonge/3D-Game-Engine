#pragma once

#include "../animation/AnimationStateMachine.h"
#include "../animation/Animator.h"
#include "../scene/Transform.h"
#include <glm/glm.hpp>
#include <memory>

// Example character controller using animation state machine
class CharacterController {
public:
    CharacterController(Animator* animator);
    ~CharacterController();

    void Update(float deltaTime);
    
    // Movement input
    void SetMovementInput(const glm::vec2& input) { m_MovementInput = input; }
    void SetSpeed(float speed) { m_Speed = speed; }
    
    // State queries
    bool IsMoving() const { return glm::length(m_MovementInput) > 0.1f; }
    float GetCurrentSpeed() const { return m_CurrentSpeed; }
    std::string GetCurrentState() const { return m_StateMachine ? m_StateMachine->GetCurrentState() : ""; }
    
    // Setup (call after loading animations)
    void SetupStates(std::shared_ptr<class AnimationClip> idle,
                    std::shared_ptr<class AnimationClip> walk,
                    std::shared_ptr<class AnimationClip> run);

private:
    void UpdateSpeed(float deltaTime);
    
    Animator* m_Animator;
    std::unique_ptr<AnimationStateMachine> m_StateMachine;
    glm::vec2 m_MovementInput = glm::vec2(0.0f);
    float m_Speed = 0.0f;
    float m_CurrentSpeed = 0.0f;
    float m_WalkSpeed = 2.0f;
    float m_RunSpeed = 5.0f;
    float m_SpeedSmoothing = 5.0f;
};

