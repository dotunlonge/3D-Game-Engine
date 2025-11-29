#pragma once

#include <glm/glm.hpp>
#include <vector>

// GPU-based skeletal animation system (uses SSBO for bone matrices)
class GPUAnimator {
public:
    GPUAnimator();
    ~GPUAnimator();

    void UploadBoneMatrices(const std::vector<glm::mat4>& boneMatrices);
    void Bind();

private:
    unsigned int m_SSBO;
    static const int MAX_BONES = 128;
};

