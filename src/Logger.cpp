#include <foxogram/Logger.h>
#include <iostream>

unsigned short foxogram::Logger::logLevel = 1;

void foxogram::Logger::logDebug(const std::string& info) {
    if (logLevel == 0) {
        std::cout << "[DEBUG] " + info << std::endl;
    }
}

void foxogram::Logger::logInfo(const std::string& info) {
    if (logLevel < 2) {
        std::cout << "\x1B[34m[INFO] " + info + "\033[0m" << std::endl;
    }
}

void foxogram::Logger::logWarning(const std::string& info) {
    if (logLevel < 3) {
        std::cout << "\x1B[33m[WARNING] " + info + "\033[0m" << std::endl;
    }
}

void foxogram::Logger::logError(const std::string& info) {
    if (logLevel < 4) {
        std::cout << "\x1B[31m[ERROR] " + info + "\033[0m" << std::endl;
    }
}

void foxogram::Logger::setLogLevel(foxogram::LogLevel level) {
    logLevel = level;
}