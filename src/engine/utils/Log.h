#pragma once
#include <string>

namespace voidx {
    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error
    };

    namespace Log {
        void SetLevel(LogLevel level);
        void Info(const std::string& message, const std::string& details = "");
        void Success(const std::string& message, const std::string& details = "");
        void Warning(const std::string& message, const std::string& details = "");
        void Error(const std::string& message, const std::string& details = "");
        void Debug(const std::string& message, const std::string& details = "");
    }
}