#pragma once

// Deferred rendering G-Buffer (placeholder for future implementation)
class GBuffer {
public:
    GBuffer(int width, int height);
    ~GBuffer();

    void Bind();
    void Unbind();
    void BindTextures();

private:
    unsigned int m_FBO;
    unsigned int m_PositionTexture;
    unsigned int m_NormalTexture;
    unsigned int m_AlbedoTexture;
    unsigned int m_DepthTexture;
    int m_Width, m_Height;
};

