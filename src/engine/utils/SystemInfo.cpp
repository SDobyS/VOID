#include "SystemInfo.h"
#include "Log.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <glad/gl.h>

namespace voidx {
    void SystemInfo::PrintHardwareInfo() {
#ifdef _WIN32
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        DWORDLONG totalRAM = memInfo.ullTotalPhys / (1024 * 1024);

        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

        Log::Info("Hardware", "RAM: " + std::to_string(totalRAM) + " MB | Cores: " + std::to_string(sysInfo.dwNumberOfProcessors));
#else
        Log::Warning("Hardware", "System info only available on Windows currently.");
#endif
    }

    void SystemInfo::PrintGPUInfo() {
        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* version = glGetString(GL_VERSION);

        std::string vStr = vendor ? reinterpret_cast<const char*>(vendor) : "Unknown";
        std::string rStr = renderer ? reinterpret_cast<const char*>(renderer) : "Unknown";
        std::string verStr = version ? reinterpret_cast<const char*>(version) : "Unknown";

        Log::Info("GPU", vStr + " | " + rStr + " | OpenGL " + verStr);
    }
}