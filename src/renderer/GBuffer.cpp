#include "GBuffer.h"
#include <glad/glad.h>

GBuffer::GBuffer(int width, int height) : m_Width(width), m_Height(height) {
    // Placeholder - full implementation would create FBO with multiple render targets
    glGenFramebuffers(1, &m_FBO);
}

GBuffer::~GBuffer() {
    glDeleteFramebuffers(1, &m_FBO);
}

void GBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void GBuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::BindTextures() {
    // Bind G-Buffer textures for lighting pass
}

