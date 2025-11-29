#include "Profiler.h"
#include <chrono>

std::unordered_map<std::string, Profiler::ProfileData> Profiler::s_Profiles;

void Profiler::Begin(const std::string& name) {
    s_Profiles[name].StartTime = std::chrono::high_resolution_clock::now();
}

void Profiler::End(const std::string& name) {
    auto it = s_Profiles.find(name);
    if (it != s_Profiles.end()) {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - it->second.StartTime);
        it->second.TotalTime += duration.count() / 1000.0f; // Convert to milliseconds
        it->second.CallCount++;
    }
}

float Profiler::GetTime(const std::string& name) {
    auto it = s_Profiles.find(name);
    if (it != s_Profiles.end()) {
        return it->second.CallCount > 0 ? it->second.TotalTime / it->second.CallCount : 0.0f;
    }
    return 0.0f;
}

void Profiler::Reset() {
    s_Profiles.clear();
}

