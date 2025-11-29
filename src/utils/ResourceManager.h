#pragma once

#include "../renderer/Shader.h"
#include "../renderer/Texture.h"
#include <string>
#include <unordered_map>
#include <memory>

class ResourceManager {
public:
    // Shader management
    static std::shared_ptr<Shader> LoadShader(const std::string& name, 
                                                const std::string& vertexPath, 
                                                const std::string& fragmentPath);
    static std::shared_ptr<Shader> GetShader(const std::string& name);
    
    // Texture management
    static std::shared_ptr<Texture> LoadTexture(const std::string& name, const std::string& path);
    static std::shared_ptr<Texture> GetTexture(const std::string& name);
    
    // Cleanup
    static void Clear();

private:
    static std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
    static std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
};

