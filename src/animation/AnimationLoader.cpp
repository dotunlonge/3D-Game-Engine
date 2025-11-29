#include "AnimationLoader.h"
#include "../core/Logger.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <glm/gtc/quaternion.hpp>

std::vector<std::shared_ptr<AnimationClip>> AnimationLoader::LoadAnimations(const std::string& filepath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, 
        aiProcess_Triangulate | 
        aiProcess_GenSmoothNormals | 
        aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG_ERROR("Failed to load scene for animation: " + filepath);
        return {};
    }
    
    return LoadAnimationsFromScene(scene);
}

std::vector<std::shared_ptr<AnimationClip>> AnimationLoader::LoadAnimationsFromScene(const aiScene* scene) {
    std::vector<std::shared_ptr<AnimationClip>> animations;
    
    if (!scene->HasAnimations()) {
        LOG_WARN("Scene has no animations");
        return animations;
    }
    
    for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
        auto clip = LoadAnimation(scene, i);
        if (clip) {
            animations.push_back(clip);
        }
    }
    
    LOG_INFO("Loaded " + std::to_string(animations.size()) + " animation(s)");
    return animations;
}

std::shared_ptr<AnimationClip> AnimationLoader::LoadAnimation(const aiScene* scene, unsigned int index) {
    if (index >= scene->mNumAnimations) {
        LOG_ERROR("Animation index out of range");
        return nullptr;
    }
    
    aiAnimation* anim = scene->mAnimations[index];
    std::string name = anim->mName.C_Str();
    float duration = (float)anim->mDuration;
    float ticksPerSecond = (float)(anim->mTicksPerSecond != 0 ? anim->mTicksPerSecond : 25.0);
    
    auto clip = std::make_shared<AnimationClip>(name, duration, ticksPerSecond);
    
    ExtractBoneAnimation(anim, *clip);
    
    return clip;
}

void AnimationLoader::ExtractBoneAnimation(aiAnimation* anim, AnimationClip& clip) {
    for (unsigned int i = 0; i < anim->mNumChannels; i++) {
        aiNodeAnim* channel = anim->mChannels[i];
        std::string boneName = channel->mNodeName.C_Str();
        
        BoneAnimation boneAnim;
        boneAnim.BoneName = boneName;
        
        // Extract position keyframes
        for (unsigned int j = 0; j < channel->mNumPositionKeys; j++) {
            Keyframe keyframe;
            keyframe.Time = (float)channel->mPositionKeys[j].mTime;
            keyframe.Position = AssimpToGLM(channel->mPositionKeys[j].mValue);
            keyframe.Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            keyframe.Scale = glm::vec3(1.0f);
            boneAnim.PositionKeys.push_back(keyframe);
        }
        
        // Extract rotation keyframes
        for (unsigned int j = 0; j < channel->mNumRotationKeys; j++) {
            Keyframe keyframe;
            keyframe.Time = (float)channel->mRotationKeys[j].mTime;
            keyframe.Position = glm::vec3(0.0f);
            keyframe.Rotation = AssimpToGLM(channel->mRotationKeys[j].mValue);
            keyframe.Scale = glm::vec3(1.0f);
            boneAnim.RotationKeys.push_back(keyframe);
        }
        
        // Extract scale keyframes
        for (unsigned int j = 0; j < channel->mNumScalingKeys; j++) {
            Keyframe keyframe;
            keyframe.Time = (float)channel->mScalingKeys[j].mTime;
            keyframe.Position = glm::vec3(0.0f);
            keyframe.Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            keyframe.Scale = AssimpToGLM(channel->mScalingKeys[j].mValue);
            boneAnim.ScaleKeys.push_back(keyframe);
        }
        
        clip.AddBoneAnimation(boneAnim);
    }
}

glm::vec3 AnimationLoader::AssimpToGLM(const aiVector3D& vec) {
    return glm::vec3(vec.x, vec.y, vec.z);
}

glm::quat AnimationLoader::AssimpToGLM(const aiQuaternion& quat) {
    return glm::quat(quat.w, quat.x, quat.y, quat.z);
}

