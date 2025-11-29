#pragma once

#include <glm/glm.hpp>
#include <memory>

// Bullet Physics wrapper (placeholder - requires Bullet integration)
class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    void StepSimulation(float deltaTime);
    void AddRigidBody(void* body);
    void RemoveRigidBody(void* body);

    // Raycast
    bool Raycast(const glm::vec3& from, const glm::vec3& to, glm::vec3& hitPoint);

private:
    void* m_DynamicsWorld; // btDiscreteDynamicsWorld*
};

