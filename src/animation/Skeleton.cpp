#include "Skeleton.h"

Skeleton::Skeleton() {
}

Skeleton::~Skeleton() {
}

int Skeleton::AddBone(const std::string& name, const glm::mat4& offsetMatrix) {
    Bone bone;
    bone.ID = m_Bones.size();
    bone.Name = name;
    bone.OffsetMatrix = offsetMatrix;
    m_Bones.push_back(bone);
    m_BoneNameMap[name] = bone.ID;
    return bone.ID;
}

void Skeleton::SetBoneParent(int boneID, int parentID) {
    if (boneID < 0 || boneID >= m_Bones.size()) return;
    if (parentID < 0) {
        m_RootBoneID = boneID;
        return;
    }
    if (parentID >= m_Bones.size()) return;
    m_Bones[parentID].Children.push_back(boneID);
}

Bone* Skeleton::GetBone(int id) {
    if (id < 0 || id >= m_Bones.size()) return nullptr;
    return &m_Bones[id];
}

Bone* Skeleton::GetBoneByName(const std::string& name) {
    auto it = m_BoneNameMap.find(name);
    if (it == m_BoneNameMap.end()) return nullptr;
    return GetBone(it->second);
}

void Skeleton::CalculateFinalTransforms() {
    if (m_RootBoneID < 0) return;
    CalculateBoneTransform(m_RootBoneID, glm::mat4(1.0f));
}

void Skeleton::CalculateBoneTransform(int boneID, const glm::mat4& parentTransform) {
    Bone& bone = m_Bones[boneID];
    glm::mat4 nodeTransform = bone.FinalTransformation; // This should be set by animator
    glm::mat4 globalTransform = parentTransform * nodeTransform;
    bone.FinalTransformation = globalTransform * bone.OffsetMatrix;

    for (int childID : bone.Children) {
        CalculateBoneTransform(childID, globalTransform);
    }
}

