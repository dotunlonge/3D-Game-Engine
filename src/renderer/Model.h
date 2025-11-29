#pragma once

#include "Mesh.h"
#include "../animation/Skeleton.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

// Forward declaration
class Texture;

class Model {
public:
    Model(const std::string& path);
    ~Model();

    void Draw(Shader& shader);
    
    // Skeleton access
    Skeleton* GetSkeleton() { return m_Skeleton.get(); }
    const std::unordered_map<std::string, int>& GetBoneMap() const { return m_BoneMap; }
    int GetBoneCount() const { return m_BoneCounter; }
    
    // Animation loading (returns animations from the loaded file)
    std::vector<std::shared_ptr<class AnimationClip>> GetAnimations() const;

private:
    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    
    // Bone extraction
    void ExtractBoneWeightsForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    int GetBoneID(const std::string& boneName);

    std::vector<Mesh> m_Meshes;
    std::string m_Directory;
    std::string m_FilePath;
    std::vector<Texture> m_TexturesLoaded; // Mesh::Texture structs
    std::vector<std::unique_ptr<class Texture>> m_TextureObjects; // Actual Texture objects
    
    // Skeleton data
    std::unique_ptr<Skeleton> m_Skeleton;
    std::unordered_map<std::string, int> m_BoneMap;
    int m_BoneCounter = 0;
    
    // Animation data
    std::vector<std::shared_ptr<AnimationClip>> m_Animations;
    
    // Helper to construct full path
    std::string GetFullPath(const std::string& path);
};

