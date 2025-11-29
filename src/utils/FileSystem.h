#pragma once

#include <string>

class FileSystem {
public:
    static std::string ReadFile(const std::string& filepath);
    static bool FileExists(const std::string& filepath);
    static std::string GetDirectory(const std::string& filepath);
    static std::string GetFileName(const std::string& filepath);
    static std::string GetExtension(const std::string& filepath);
};

