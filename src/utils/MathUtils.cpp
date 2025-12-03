#include "MathUtils.h"
#include <algorithm>

float MathUtils::SmoothStep(float edge0, float edge1, float x) {
    x = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return x * x * (3.0f - 2.0f * x);
}

float MathUtils::MapRange(float value, float inMin, float inMax, float outMin, float outMax) {
    return outMin + (value - inMin) * (outMax - outMin) / (inMax - inMin);
}

float MathUtils::DegreesToRadians(float degrees) {
    return degrees * 3.14159265359f / 180.0f;
}

float MathUtils::RadiansToDegrees(float radians) {
    return radians * 180.0f / 3.14159265359f;
}

float MathUtils::Distance(const glm::vec3& a, const glm::vec3& b) {
    return glm::length(b - a);
}

float MathUtils::DistanceSquared(const glm::vec3& a, const glm::vec3& b) {
    glm::vec3 diff = b - a;
    return glm::dot(diff, diff);
}

glm::vec3 MathUtils::Normalize(const glm::vec3& v) {
    return glm::normalize(v);
}

glm::quat MathUtils::Slerp(const glm::quat& a, const glm::quat& b, float t) {
    return glm::slerp(a, b, t);
}

glm::mat4 MathUtils::LookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
    return glm::lookAt(eye, center, up);
}

