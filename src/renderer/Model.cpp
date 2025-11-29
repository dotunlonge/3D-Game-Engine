#include "Model.h"
#include "../core/Logger.h"
#include "../renderer/Texture.h"
#include "../animation/AnimationLoader.h"
#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

Model::Model(const std::string& path) {
    m_FilePath = path;
    m_Skeleton = std::make_unique<Skeleton>();
    LoadModel(path);
}

Model::~Model() {
}

void Model::LoadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, 
        aiProcess_Triangulate | 
        aiProcess_GenSmoothNormals | 
        aiProcess_FlipUVs | 
        aiProcess_CalcTangentSpace |
        aiProcess_LimitBoneWeights);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG_ERROR("ERROR::ASSIMP:: " + std::string(importer.GetErrorString()));
        return;
    }
    m_Directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
    
    // Extract animations from scene
    if (scene->HasAnimations()) {
        LOG_INFO("Model has " + std::to_string(scene->mNumAnimations) + " animation(s)");
        m_Animations = AnimationLoader::LoadAnimationsFromScene(scene);
    }
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        // Initialize bone data
        vertex.BoneIDs = glm::ivec4(0);
        vertex.BoneWeights = glm::vec4(0.0f);

        vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Extract bone weights
    if (mesh->HasBones()) {
        ExtractBoneWeightsForVertices(vertices, mesh, scene);
    }

    // Process materials
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

void Model::ExtractBoneWeightsForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene) {
    for (unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++) {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        
        if (m_BoneMap.find(boneName) == m_BoneMap.end()) {
            // New bone - add to skeleton
            aiMatrix4x4 offsetMatrix = mesh->mBones[boneIndex]->mOffsetMatrix;
            glm::mat4 glmOffsetMatrix;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    glmOffsetMatrix[i][j] = offsetMatrix[j][i]; // Assimp is row-major, GLM is column-major
                }
            }
            
            boneID = m_Skeleton->AddBone(boneName, glmOffsetMatrix);
            m_BoneMap[boneName] = boneID;
            m_BoneCounter++;
        } else {
            boneID = m_BoneMap[boneName];
        }
        
        // Assign weights to vertices
        aiBone* bone = mesh->mBones[boneIndex];
        for (unsigned int weightIndex = 0; weightIndex < bone->mNumWeights; weightIndex++) {
            int vertexID = bone->mWeights[weightIndex].mVertexId;
            float weight = bone->mWeights[weightIndex].mWeight;
            SetVertexBoneData(vertices[vertexID], boneID, weight);
        }
    }
}

void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight) {
    for (int i = 0; i < 4; i++) {
        if (vertex.BoneWeights[i] == 0.0f) {
            vertex.BoneIDs[i] = boneID;
            vertex.BoneWeights[i] = weight;
            return;
        }
    }
    // If we get here, vertex already has 4 bones - log warning
    // In production, you'd want to keep the highest weights
}

int Model::GetBoneID(const std::string& boneName) {
    auto it = m_BoneMap.find(boneName);
    if (it != m_BoneMap.end()) {
        return it->second;
    }
    return -1;
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        
        // Check if texture already loaded
        bool skip = false;
        std::string texturePath = GetFullPath(str.C_Str());
        
        for (unsigned int j = 0; j < m_TexturesLoaded.size(); j++) {
            if (m_TexturesLoaded[j].path == texturePath) {
                textures.push_back(m_TexturesLoaded[j]);
                skip = true;
                break;
            }
        }
        
        if (!skip) {
            // Try to load texture
            std::ifstream file(texturePath);
            if (file.good()) {
                file.close();
                try {
                    // Load texture using Texture class
                    auto textureObj = std::make_unique<class Texture>(texturePath);
                    unsigned int textureID = textureObj->GetID();
                    
                    if (textureID != 0) {
                        // Store texture object to keep it alive
                        m_TextureObjects.push_back(std::move(textureObj));
                        
                        // Create Mesh::Texture struct
                        Texture meshTexture;
                        meshTexture.id = textureID;
                        meshTexture.type = typeName;
                        meshTexture.path = texturePath;
                        textures.push_back(meshTexture);
                        m_TexturesLoaded.push_back(meshTexture);
                    } else {
                        LOG_WARN("Failed to load texture (ID=0): " + texturePath);
                    }
                } catch (const std::exception& e) {
                    LOG_ERROR("Exception loading texture: " + texturePath + " - " + e.what());
                } catch (...) {
                    LOG_ERROR("Unknown error loading texture: " + texturePath);
                }
            } else {
                LOG_WARN("Texture file not found: " + texturePath);
            }
        }
    }
    return textures;
}

std::string Model::GetFullPath(const std::string& path) {
    // If path is absolute, return as-is
    if (path[0] == '/' || (path.length() > 1 && path[1] == ':')) {
        return path;
    }
    // Otherwise, prepend directory
    return m_Directory + "/" + path;
}

void Model::Draw(Shader& shader) {
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
        m_Meshes[i].Draw(shader);
}

std::vector<std::shared_ptr<AnimationClip>> Model::GetAnimations() const {
    return m_Animations;
}

