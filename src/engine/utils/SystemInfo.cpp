#include "SystemInfo.h"
#include "Log.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <glad/gl.h>

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
    std::string vendor   = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    std::string renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    std::string version  = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    Log::Info("GPU", vendor + " | " + renderer + " | OpenGL " + version);
}