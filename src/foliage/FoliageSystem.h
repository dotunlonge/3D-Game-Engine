#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "../renderer/Mesh.h"

struct FoliageInstance {
    glm::mat4 Transform;
    float LODDistance;
};

class FoliageSystem {
public:
    FoliageSystem();
    ~FoliageSystem();

    void AddInstance(const glm::vec3& position, float scale = 1.0f);
    void CullInstances(const glm::vec3& cameraPosition, float maxDistance);
    void RenderInstanced(Mesh& mesh, unsigned int shaderID);

private:
    std::vector<FoliageInstance> m_Instances;
    std::vector<FoliageInstance> m_VisibleInstances;
    unsigned int m_InstanceVBO;
    static const float LOD_DISTANCE_1;
    static const float LOD_DISTANCE_2;
};

