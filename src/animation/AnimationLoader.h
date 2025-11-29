#pragma once

#include "AnimationClip.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <string>
#include <memory>
#include <vector>

// Utility class to load animations from Assimp scenes
class AnimationLoader {
public:
    // Load all animations from a file
    static std::vector<std::shared_ptr<AnimationClip>> LoadAnimations(const std::string& filepath);
    
    // Load animations from an Assimp scene
    static std::vector<std::shared_ptr<AnimationClip>> LoadAnimationsFromScene(const aiScene* scene);
    
    // Load a single animation by index
    static std::shared_ptr<AnimationClip> LoadAnimation(const aiScene* scene, unsigned int index);

private:
    static void ExtractBoneAnimation(aiAnimation* anim, AnimationClip& clip);
    static glm::vec3 AssimpToGLM(const aiVector3D& vec);
    static glm::quat AssimpToGLM(const aiQuaternion& quat);
};

