#include <foxochat/Logger.h>
#include <iostream>

unsigned short foxochat::Logger::logLevel = 1;

void foxochat::Logger::logDebug(const std::string& info) {
    if (logLevel == 0) {
        std::cout << "[DEBUG] " + info << std::endl;
    }
}

void foxochat::Logger::logInfo(const std::string& info) {
    if (logLevel < 2) {
        std::cout << "\033[0;34m[INFO] " + info + "\033[0m" << std::endl;
    }
}

void foxochat::Logger::logWarning(const std::string& info) {
    if (logLevel < 3) {
        std::cout << "\033[0;33m[WARNING] " + info + "\033[0m" << std::endl;
    }
}

void foxochat::Logger::logError(const std::string& info) {
    if (logLevel < 4) {
        std::cout << "\033[0;31m[ERROR] " + info + "\033[0m" << std::endl;
    }
}

void foxochat::Logger::setLogLevel(foxochat::LogLevel level) {
    logLevel = level;
}