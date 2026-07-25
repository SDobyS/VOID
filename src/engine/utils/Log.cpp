#include "Log.h"
#include "ConsoleColors.h"
#include "Time.h"
#include <iostream>

namespace Log {
    static void PrintLog(const std::string& color, const std::string& tag, const std::string& message, const std::string& details) {
        if (details.empty()) {
            std::cout
            << "[" << GetCurrentTime() << "] "
            << color << "[" << tag << "] "
            << Colors::Reset << message
            << std::endl;
        } else {
            std::cout
            << "[" << GetCurrentTime() << "] "
            << color << "[" << tag << "] "
            << Colors::Reset
            << message << ": " << details
            << std::endl;
        }
    }

    void Info(const    std::string& message, const std::string& details)  { PrintLog(Colors::Cyan,    "INFO", message, details); }
    void Success(const std::string& message, const std::string& details)  { PrintLog(Colors::Green,   " OK ", message, details); }
    void Warning(const std::string& message, const std::string& details)  { PrintLog(Colors::Yellow,  "WARN", message, details); }
    void Error(const   std::string& message, const std::string& details)  { PrintLog(Colors::Red,     "ERRO", message, details); }
    void Debug(const   std::string& message, const std::string& details)  { PrintLog(Colors::Magenta, "DEBG", message, details); }
}
