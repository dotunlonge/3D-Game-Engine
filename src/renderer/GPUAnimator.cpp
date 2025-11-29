#include "GPUAnimator.h"
#include <glad/glad.h>

GPUAnimator::GPUAnimator() {
    glGenBuffers(1, &m_SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_BONES * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

GPUAnimator::~GPUAnimator() {
    glDeleteBuffers(1, &m_SSBO);
}

void GPUAnimator::UploadBoneMatrices(const std::vector<glm::mat4>& boneMatrices) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, boneMatrices.size() * sizeof(glm::mat4), boneMatrices.data());
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void GPUAnimator::Bind() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO);
}

