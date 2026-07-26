#include "Log.h"
#include "ConsoleColors.h"
#include "Time.h"
#include <iostream>

namespace voidx {
    namespace Log {
        static LogLevel s_CurrentLevel = LogLevel::Debug;

        void SetLevel(LogLevel level) { s_CurrentLevel = level; }

        static void PrintLog(LogLevel level, const std::string& color, const std::string& tag, const std::string& message, const std::string& details) {
            if (level < s_CurrentLevel) return;

            if (details.empty()) {
                std::cout << "[" << GetCurrentTime() << "] " << color << "[" << tag << "] " << Colors::Reset << message << std::endl;
            } else {
                std::cout << "[" << GetCurrentTime() << "] " << color << "[" << tag << "] " << Colors::Reset << message << ": " << details << std::endl;
            }
        }

        void Info(const std::string& message, const std::string& details)  { PrintLog(LogLevel::Info, Colors::Cyan, "INFO", message, details); }
        void Success(const std::string& message, const std::string& details) { PrintLog(LogLevel::Info, Colors::Green, " OK ", message, details); }
        void Warning(const std::string& message, const std::string& details) { PrintLog(LogLevel::Warning, Colors::Yellow, "WARN", message, details); }
        void Error(const std::string& message, const std::string& details)   { PrintLog(LogLevel::Error, Colors::Red, "ERRO", message, details); }
        void Debug(const std::string& message, const std::string& details)   { PrintLog(LogLevel::Debug, Colors::Magenta, "DEBG", message, details); }
    }
}