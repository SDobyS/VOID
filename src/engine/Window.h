#pragma once

enum class VSyncMode {
    Disabled = 0,
    Enabled = 1,
    Adaptive = 2
};

struct WindowConfig {
    const char* title = "VOID";
    int x = -1;
    int y = -1;
    int width = 1280;
    int height = 720;

    int minWidth = 320;
    int minHeight = 180;
    int maxWidth = 0;
    int maxHeight = 0;

    bool resizable = true;
    bool fullscreen = false;
    bool borderless = false;
    bool highDPI = true;

    VSyncMode vsync = VSyncMode::Enabled;
    int fpsLimit = 0;

    int glMajor = 4;
    int glMinor = 6;

    int depthBits = 24;
    int stencilBits = 8;
    int samples = 0;

    const char* iconPath = nullptr;
    bool showDebugUI = false;
};

bool InitWindow(const WindowConfig& config);
void CloseWindow();
bool WindowShouldClose();

void BeginDrawing();
void EndDrawing();

void SetWindowSize(int width, int height);
void ToggleFullscreen();
bool IsDebugUIVisible();

float GetDeltaTime();
float GetFrameTime();
int GetFPS();

int GetWindowWidth();
int GetWindowHeight();