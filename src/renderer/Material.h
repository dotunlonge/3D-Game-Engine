#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>

class Texture;

// Simple material class for managing material properties
class Material {
public:
    Material();
    ~Material();

    // Albedo/Diffuse
    void SetAlbedo(const glm::vec3& color) { m_Albedo = color; }
    glm::vec3 GetAlbedo() const { return m_Albedo; }
    void SetAlbedoTexture(std::shared_ptr<Texture> texture) { m_AlbedoTexture = texture; }
    std::shared_ptr<Texture> GetAlbedoTexture() const { return m_AlbedoTexture; }
    
    // Metallic/Roughness (for PBR)
    void SetMetallic(float metallic) { m_Metallic = metallic; }
    float GetMetallic() const { return m_Metallic; }
    void SetRoughness(float roughness) { m_Roughness = roughness; }
    float GetRoughness() const { return m_Roughness; }
    
    // Emission
    void SetEmission(const glm::vec3& color) { m_Emission = color; }
    glm::vec3 GetEmission() const { return m_Emission; }
    
    // Name
    void SetName(const std::string& name) { m_Name = name; }
    std::string GetName() const { return m_Name; }

private:
    std::string m_Name = "DefaultMaterial";
    glm::vec3 m_Albedo = glm::vec3(1.0f);
    std::shared_ptr<Texture> m_AlbedoTexture;
    float m_Metallic = 0.0f;
    float m_Roughness = 0.5f;
    glm::vec3 m_Emission = glm::vec3(0.0f);
};

