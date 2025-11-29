#include "ResourceManager.h"
#include "../core/Logger.h"

std::unordered_map<std::string, std::shared_ptr<Shader>> ResourceManager::m_Shaders;
std::unordered_map<std::string, std::shared_ptr<Texture>> ResourceManager::m_Textures;

std::shared_ptr<Shader> ResourceManager::LoadShader(const std::string& name, 
                                                     const std::string& vertexPath, 
                                                     const std::string& fragmentPath) {
    auto it = m_Shaders.find(name);
    if (it != m_Shaders.end()) {
        LOG_WARN("Shader '" + name + "' already loaded. Returning existing shader.");
        return it->second;
    }
    
    try {
        auto shader = std::make_shared<Shader>(vertexPath, fragmentPath);
        m_Shaders[name] = shader;
        LOG_INFO("Loaded shader: " + name);
        return shader;
    } catch (...) {
        LOG_ERROR("Failed to load shader: " + name);
        return nullptr;
    }
}

std::shared_ptr<Shader> ResourceManager::GetShader(const std::string& name) {
    auto it = m_Shaders.find(name);
    if (it != m_Shaders.end()) {
        return it->second;
    }
    LOG_WARN("Shader '" + name + "' not found.");
    return nullptr;
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(const std::string& name, const std::string& path) {
    auto it = m_Textures.find(name);
    if (it != m_Textures.end()) {
        LOG_WARN("Texture '" + name + "' already loaded. Returning existing texture.");
        return it->second;
    }
    
    try {
        auto texture = std::make_shared<Texture>(path);
        m_Textures[name] = texture;
        LOG_INFO("Loaded texture: " + name + " from " + path);
        return texture;
    } catch (...) {
        LOG_ERROR("Failed to load texture: " + name + " from " + path);
        return nullptr;
    }
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& name) {
    auto it = m_Textures.find(name);
    if (it != m_Textures.end()) {
        return it->second;
    }
    LOG_WARN("Texture '" + name + "' not found.");
    return nullptr;
}

void ResourceManager::Clear() {
    m_Shaders.clear();
    m_Textures.clear();
    LOG_INFO("ResourceManager cleared.");
}

