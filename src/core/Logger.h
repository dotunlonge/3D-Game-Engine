#pragma once

#include <iostream>
#include <sstream>

#define LOG_INFO(msg)    Logger::Log(Logger::Level::Info, msg)
#define LOG_WARN(msg)    Logger::Log(Logger::Level::Warning, msg)
#define LOG_ERROR(msg)   Logger::Log(Logger::Level::Error, msg)
#define LOG_DEBUG(msg)   Logger::Log(Logger::Level::Debug, msg)

class Logger {
public:
    enum class Level {
        Info,
        Warning,
        Error,
        Debug
    };

    static void Log(Level level, const std::string& message);
};

