#ifndef LOG_LEVEL_HPP
#define LOG_LEVEL_HPP

#include <string>

enum class LogLevel{
    TRACE,
    DEBUG, 
    INFO, 
    WARN, 
    ERROR, 
    FATAL
};

inline std::string logLevelToString(LogLevel level){
    switch(level){
        case LogLevel::TRACE: return "TRACE"; 
        case LogLevel::DEBUG: return "DEBUG"; 
        case LogLevel::INFO: return "INFO"; 
        case LogLevel::WARN: return "WARN"; 
        case LogLevel::ERROR: return "ERROR"; 
        case LogLevel::FATAL: return "FATAL"; 
        default: return "UNKNOWN"; 
    }
}

#endif 