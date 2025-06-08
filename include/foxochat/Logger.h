#pragma once
#include <foxochat/export.h>
#include <foxochat/Enums.h>
#include <string>

namespace foxochat {
    class LIBFOXOCHAT_EXPORT Logger {
    public:
        static unsigned short logLevel;
        static void setLogLevel(LogLevel level);
        static void logDebug(const std::string& info);
        static void logInfo(const std::string& info);
        static void logWarning(const std::string& info);
        static void logError(const std::string& info);
    };
}