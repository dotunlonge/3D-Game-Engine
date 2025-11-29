#include "GPUAnimator.h"
#include "../core/Logger.h"
#include <glad/glad.h>
#include <algorithm>

GPUAnimator::GPUAnimator() {
    glGenBuffers(1, &m_SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
    // Allocate space for max bones
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_BONES * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

GPUAnimator::~GPUAnimator() {
    if (m_SSBO != 0) {
        glDeleteBuffers(1, &m_SSBO);
    }
}

void GPUAnimator::UploadBoneMatrices(const std::vector<glm::mat4>& boneMatrices) {
    if (boneMatrices.empty()) return;
    
    size_t boneCount = std::min(boneMatrices.size(), (size_t)MAX_BONES);
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
    
    // Upload bone matrices
    if (boneCount > 0) {
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 
                       boneCount * sizeof(glm::mat4), 
                       boneMatrices.data());
    }
    
    // Fill remaining with identity matrices if needed
    if (boneCount < MAX_BONES) {
        std::vector<glm::mat4> identityMatrices(MAX_BONES - boneCount, glm::mat4(1.0f));
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 
                       boneCount * sizeof(glm::mat4),
                       (MAX_BONES - boneCount) * sizeof(glm::mat4),
                       identityMatrices.data());
    }
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void GPUAnimator::Bind() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO);
}

void GPUAnimator::Unbind() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
}

