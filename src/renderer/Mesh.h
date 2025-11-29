#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::ivec4 BoneIDs = glm::ivec4(0);
    glm::vec4 BoneWeights = glm::vec4(0.0f);
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    ~Mesh();

    void Draw(Shader& shader);

private:
    void SetupMesh();

    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<Texture> m_Textures;
    unsigned int m_VAO, m_VBO, m_EBO;
};

