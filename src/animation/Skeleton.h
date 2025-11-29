#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Bone {
    int ID;
    std::string Name;
    glm::mat4 OffsetMatrix;
    glm::mat4 FinalTransformation;
    std::vector<int> Children;
};

class Skeleton {
public:
    Skeleton();
    ~Skeleton();

    int AddBone(const std::string& name, const glm::mat4& offsetMatrix);
    void SetBoneParent(int boneID, int parentID);
    Bone* GetBone(int id);
    Bone* GetBoneByName(const std::string& name);
    int GetBoneCount() const { return m_Bones.size(); }

    void CalculateFinalTransforms();

private:
    void CalculateBoneTransform(int boneID, const glm::mat4& parentTransform);

    std::vector<Bone> m_Bones;
    std::unordered_map<std::string, int> m_BoneNameMap;
    int m_RootBoneID = -1;
};

