#include "Logger.h"
#include <iostream>

void Logger::Log(Level level, const std::string& message) {
    switch (level) {
        case Level::Info:
            std::cout << "[INFO] " << message << std::endl;
            break;
        case Level::Warning:
            std::cout << "[WARN] " << message << std::endl;
            break;
        case Level::Error:
            std::cerr << "[ERROR] " << message << std::endl;
            break;
        case Level::Debug:
            std::cout << "[DEBUG] " << message << std::endl;
            break;
    }
}

