#version 450 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aUV;
layout(location=3) in vec4 aBoneIDs;
layout(location=4) in vec4 aBoneWeights;

uniform mat4 uViewProj;
uniform mat4 uModel;

// SSBO for bone matrices
layout(std430, binding = 0) readonly buffer BoneMatrices {
    mat4 uBones[];
};

mat4 getSkinMatrix() {
    mat4 skin = mat4(0.0);
    if (aBoneWeights.x > 0.0) {
        skin += uBones[int(aBoneIDs.x)] * aBoneWeights.x;
    }
    if (aBoneWeights.y > 0.0) {
        skin += uBones[int(aBoneIDs.y)] * aBoneWeights.y;
    }
    if (aBoneWeights.z > 0.0) {
        skin += uBones[int(aBoneIDs.z)] * aBoneWeights.z;
    }
    if (aBoneWeights.w > 0.0) {
        skin += uBones[int(aBoneIDs.w)] * aBoneWeights.w;
    }
    return skin;
}

out vec2 vUV;
out vec3 vNormal;
out vec3 vWorldPos;

void main() {
    mat4 skinMatrix = getSkinMatrix();
    mat4 model = skinMatrix * uModel;
    
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vUV = aUV;
    vec4 worldPos = model * vec4(aPos, 1.0);
    vWorldPos = worldPos.xyz;
    gl_Position = uViewProj * worldPos;
}

