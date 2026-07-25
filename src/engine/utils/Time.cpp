#include "Time.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

std::string GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf{};

#if defined(_MSC_VER)
    localtime_s(&tm_buf, &time);
#else
    localtime_r(&time, &tm_buf);
#endif

    std::stringstream ss;
    ss << std::put_time(&tm_buf, "%H:%M:%S");
    return ss.str();
}
