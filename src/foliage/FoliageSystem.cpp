#include "FoliageSystem.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

const float FoliageSystem::LOD_DISTANCE_1 = 50.0f;
const float FoliageSystem::LOD_DISTANCE_2 = 100.0f;

FoliageSystem::FoliageSystem() {
    glGenBuffers(1, &m_InstanceVBO);
}

FoliageSystem::~FoliageSystem() {
    glDeleteBuffers(1, &m_InstanceVBO);
}

void FoliageSystem::AddInstance(const glm::vec3& position, float scale) {
    FoliageInstance instance;
    instance.Transform = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(scale));
    instance.LODDistance = 0.0f;
    m_Instances.push_back(instance);
}

void FoliageSystem::CullInstances(const glm::vec3& cameraPosition, float maxDistance) {
    m_VisibleInstances.clear();
    for (const auto& instance : m_Instances) {
        glm::vec3 instancePos = glm::vec3(instance.Transform[3]);
        float distance = glm::length(instancePos - cameraPosition);
        if (distance <= maxDistance) {
            FoliageInstance visible = instance;
            visible.LODDistance = distance;
            m_VisibleInstances.push_back(visible);
        }
    }
}

void FoliageSystem::RenderInstanced(Mesh& mesh, unsigned int shaderID) {
    if (m_VisibleInstances.empty()) return;

    // Update instance buffer
    std::vector<glm::mat4> transforms;
    for (const auto& instance : m_VisibleInstances) {
        transforms.push_back(instance.Transform);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(glm::mat4), transforms.data(), GL_DYNAMIC_DRAW);

    // Draw instanced (requires mesh to support instanced rendering)
    // glDrawElementsInstanced(...)
}

