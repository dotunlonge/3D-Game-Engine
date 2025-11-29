#pragma once

#include "Mesh.h"
#include <string>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class Model {
public:
    Model(const std::string& path);
    ~Model();

    void Draw(Shader& shader);

private:
    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    std::vector<Mesh> m_Meshes;
    std::string m_Directory;
    std::vector<Texture> m_TexturesLoaded;
};

