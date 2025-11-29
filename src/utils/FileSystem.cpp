#include "FileSystem.h"
#include <fstream>
#include <sstream>
#include <filesystem>

std::string FileSystem::ReadFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool FileSystem::FileExists(const std::string& filepath) {
    return std::filesystem::exists(filepath);
}

std::string FileSystem::GetDirectory(const std::string& filepath) {
    size_t found = filepath.find_last_of("/\\");
    if (found != std::string::npos) {
        return filepath.substr(0, found);
    }
    return "";
}

std::string FileSystem::GetFileName(const std::string& filepath) {
    size_t found = filepath.find_last_of("/\\");
    if (found != std::string::npos) {
        return filepath.substr(found + 1);
    }
    return filepath;
}

std::string FileSystem::GetExtension(const std::string& filepath) {
    size_t found = filepath.find_last_of(".");
    if (found != std::string::npos) {
        return filepath.substr(found + 1);
    }
    return "";
}

