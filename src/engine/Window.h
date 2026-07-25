#pragma once

struct WindowConfig {
    const char* title = "VOID";
    int width = 1280;
    int height = 720;
    bool resizable = true;
    bool vsync = true;
    int glMajor = 4;
    int glMinor = 6;
};

bool InitWindow(const WindowConfig& config);
void CloseWindow();
bool WindowShouldClose();

void BeginDrawing();
void EndDrawing();

float GetDeltaTime();
int GetFPS();

int GetWindowWidth();
int GetWindowHeight();
