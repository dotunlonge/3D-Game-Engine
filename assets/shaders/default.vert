#version 450 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aUV;
layout(location=3) in vec4 aBoneIDs;    // if use skinning
layout(location=4) in vec4 aBoneWeights;

uniform mat4 uViewProj;
uniform mat4 uModel;

#ifdef USE_SKINNING
layout(std140, binding = 0) uniform Bones {
    mat4 uBones[128];
};
mat4 getSkinMatrix() {
    mat4 skin = mat4(0.0);
    skin += uBones[int(aBoneIDs.x)] * aBoneWeights.x;
    skin += uBones[int(aBoneIDs.y)] * aBoneWeights.y;
    skin += uBones[int(aBoneIDs.z)] * aBoneWeights.z;
    skin += uBones[int(aBoneIDs.w)] * aBoneWeights.w;
    return skin;
}
#endif

out vec2 vUV;
out vec3 vNormal;
out vec3 vWorldPos;

void main() {
    mat4 model = uModel;
    #ifdef USE_SKINNING
        model = getSkinMatrix() * uModel;
    #endif
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vUV = aUV;
    vec4 worldPos = model * vec4(aPos, 1.0);
    vWorldPos = worldPos.xyz;
    gl_Position = uViewProj * worldPos;
}

