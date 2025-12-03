#include "CharacterController.h"
#include "../core/Logger.h"
#include "../animation/AnimationClip.h"

CharacterController::CharacterController(Animator* animator) : m_Animator(animator) {
    if (animator) {
        m_StateMachine = std::make_unique<AnimationStateMachine>(animator);
    }
}

CharacterController::~CharacterController() {
}

void CharacterController::SetupStates(std::shared_ptr<AnimationClip> idle,
                                     std::shared_ptr<AnimationClip> walk,
                                     std::shared_ptr<AnimationClip> run) {
    if (!m_StateMachine) {
        LOG_ERROR("CharacterController: State machine not initialized");
        return;
    }
    
    // Add states
    m_StateMachine->AddState("Idle", idle, true);
    m_StateMachine->AddState("Walk", walk, true);
    m_StateMachine->AddState("Run", run, true);
    
    // Set default state
    m_StateMachine->SetDefaultState("Idle");
    
    // Add transitions
    // Idle -> Walk
    m_StateMachine->AddTransition("Idle", "Walk", [this]() {
        return IsMoving() && m_CurrentSpeed < m_RunSpeed * 0.7f;
    }, 0.2f);
    
    // Walk -> Idle
    m_StateMachine->AddTransition("Walk", "Idle", [this]() {
        return !IsMoving() || m_CurrentSpeed < 0.1f;
    }, 0.2f);
    
    // Walk -> Run
    m_StateMachine->AddTransition("Walk", "Run", [this]() {
        return IsMoving() && m_CurrentSpeed >= m_RunSpeed * 0.7f;
    }, 0.2f);
    
    // Run -> Walk
    m_StateMachine->AddTransition("Run", "Walk", [this]() {
        return IsMoving() && m_CurrentSpeed < m_RunSpeed * 0.7f;
    }, 0.2f);
    
    // Run -> Idle
    m_StateMachine->AddTransition("Run", "Idle", [this]() {
        return !IsMoving() || m_CurrentSpeed < 0.1f;
    }, 0.2f);
}

void CharacterController::Update(float deltaTime) {
    if (!m_StateMachine) return;
    
    // Update speed based on movement input
    UpdateSpeed(deltaTime);
    
    // Update state machine (evaluates transitions)
    m_StateMachine->Update(deltaTime);
}

void CharacterController::UpdateSpeed(float deltaTime) {
    float targetSpeed = 0.0f;
    
    if (IsMoving()) {
        float inputMagnitude = glm::length(m_MovementInput);
        if (m_Speed > m_RunSpeed * 0.7f) {
            targetSpeed = m_RunSpeed * inputMagnitude;
        } else {
            targetSpeed = m_WalkSpeed * inputMagnitude;
        }
    }
    
    // Smooth speed transition
    float speedDiff = targetSpeed - m_CurrentSpeed;
    m_CurrentSpeed += speedDiff * m_SpeedSmoothing * deltaTime;
    
    // Clamp
    if (m_CurrentSpeed < 0.01f) m_CurrentSpeed = 0.0f;
    if (m_CurrentSpeed > m_RunSpeed) m_CurrentSpeed = m_RunSpeed;
}

