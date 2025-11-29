#pragma once

#include <string>
#include <unordered_map>
#include <chrono>

class Profiler {
public:
    static void Begin(const std::string& name);
    static void End(const std::string& name);
    static float GetTime(const std::string& name);
    static void Reset();

private:
    struct ProfileData {
        std::chrono::high_resolution_clock::time_point StartTime;
        float TotalTime = 0.0f;
        int CallCount = 0;
    };

    static std::unordered_map<std::string, ProfileData> s_Profiles;
};

