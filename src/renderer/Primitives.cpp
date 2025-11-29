#include "Primitives.h"
#include <glm/glm.hpp>
#include <cmath>

Mesh Primitives::CreateCube() {
    std::vector<Vertex> vertices = {
        // Front face
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
        // Back face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
    };
    
    std::vector<unsigned int> indices = {
        // Front
        0, 1, 2, 2, 3, 0,
        // Back
        4, 5, 6, 6, 7, 4,
        // Left
        7, 3, 0, 0, 4, 7,
        // Right
        1, 5, 6, 6, 2, 1,
        // Top
        3, 2, 6, 6, 7, 3,
        // Bottom
        0, 1, 5, 5, 4, 0
    };
    
    return Mesh(vertices, indices, {});
}

Mesh Primitives::CreateQuad() {
    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
    };
    
    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0
    };
    
    return Mesh(vertices, indices, {});
}

Mesh Primitives::CreatePlane(float width, float height) {
    float w = width / 2.0f;
    float h = height / 2.0f;
    
    std::vector<Vertex> vertices = {
        {{-w, 0.0f, -h}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
        {{ w, 0.0f, -h}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
        {{ w, 0.0f,  h}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
        {{-w, 0.0f,  h}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f}},
    };
    
    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0
    };
    
    return Mesh(vertices, indices, {});
}

Mesh Primitives::CreateSphere(int segments) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Generate sphere vertices
    for (int y = 0; y <= segments; y++) {
        for (int x = 0; x <= segments; x++) {
            float xSegment = (float)x / (float)segments;
            float ySegment = (float)y / (float)segments;
            float xPos = std::cos(xSegment * 2.0f * 3.14159f) * std::sin(ySegment * 3.14159f);
            float yPos = std::cos(ySegment * 3.14159f);
            float zPos = std::sin(xSegment * 2.0f * 3.14159f) * std::sin(ySegment * 3.14159f);
            
            Vertex vertex;
            vertex.Position = glm::vec3(xPos, yPos, zPos) * 0.5f;
            vertex.Normal = glm::normalize(glm::vec3(xPos, yPos, zPos));
            vertex.TexCoords = glm::vec2(xSegment, ySegment);
            vertex.BoneIDs = glm::ivec4(0);
            vertex.BoneWeights = glm::vec4(0.0f);
            vertices.push_back(vertex);
        }
    }
    
    // Generate indices
    for (int y = 0; y < segments; y++) {
        for (int x = 0; x < segments; x++) {
            int first = (y * (segments + 1)) + x;
            int second = first + segments + 1;
            
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);
            
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
    
    return Mesh(vertices, indices, {});
}

