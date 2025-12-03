#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cmath>

// Utility math functions
class MathUtils {
public:
    // Clamp value between min and max
    template<typename T>
    static T Clamp(T value, T min, T max) {
        return std::max(min, std::min(max, value));
    }
    
    // Linear interpolation
    template<typename T>
    static T Lerp(T a, T b, float t) {
        return a + (b - a) * t;
    }
    
    // Smooth step interpolation
    static float SmoothStep(float edge0, float edge1, float x);
    
    // Map value from one range to another
    static float MapRange(float value, float inMin, float inMax, float outMin, float outMax);
    
    // Angle conversion
    static float DegreesToRadians(float degrees);
    static float RadiansToDegrees(float radians);
    
    // Vector utilities
    static float Distance(const glm::vec3& a, const glm::vec3& b);
    static float DistanceSquared(const glm::vec3& a, const glm::vec3& b);
    static glm::vec3 Normalize(const glm::vec3& v);
    
    // Quaternion utilities
    static glm::quat Slerp(const glm::quat& a, const glm::quat& b, float t);
    
    // Matrix utilities
    static glm::mat4 LookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
};

