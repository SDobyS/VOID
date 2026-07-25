#include "Window.h"

#include "ImGuiLayer.h"
#include "utils/Log.h"
#include "utils/SystemInfo.h"
#include "graphics/Renderer.h"

#include <SDL3/SDL.h>
#include <glad/gl.h>
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <chrono>

static SDL_Window* s_Window = nullptr;
static SDL_GLContext s_GLContext = nullptr;
static bool s_Running = true;
static int s_Width = 0;
static int s_Height = 0;

static std::chrono::time_point<std::chrono::system_clock> s_FrameStart;
static float s_DeltaTime = 0.0f;
static int s_CurrentFPS = 0;

bool InitWindow(const WindowConfig& config) {
    Log::Info("Starting VOID Engine...");
    if (!SDL_Init(SDL_INIT_VIDEO)) { Log::Error("SDL init failed", SDL_GetError()); return false; }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, config.glMajor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, config.glMinor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    Uint64 flags = SDL_WINDOW_OPENGL;
    if (config.resizable) flags |= SDL_WINDOW_RESIZABLE;

    s_Window = SDL_CreateWindow(config.title, config.width, config.height, flags);
    if (!s_Window) { Log::Error("Window creation failed", SDL_GetError()); return false; }
    s_Width = config.width; s_Height = config.height;
    Log::Success("Window created");

    s_GLContext = SDL_GL_CreateContext(s_Window);
    if (!s_GLContext) { Log::Error("GL Context failed", SDL_GetError()); return false; }
    SDL_GL_MakeCurrent(s_Window, s_GLContext);
    SDL_GL_SetSwapInterval(config.vsync ? 1 : 0);

    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) { Log::Error("GLAD init failed"); return false; }
    Log::Success("GLAD initialized");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    SystemInfo::PrintHardwareInfo();
    SystemInfo::PrintGPUInfo();
    Renderer::Init();

    if (!InitImGui(s_Window, s_GLContext)) return false;
    return true;
}

void BeginDrawing() {
    s_FrameStart = std::chrono::system_clock::now();
    SDL_GetWindowSize(s_Window, &s_Width, &s_Height);
    glViewport(0, 0, s_Width, s_Height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT) s_Running = false;
    }
    BeginImGui();
}

void EndDrawing() {
    EndImGui();
    SDL_GL_SwapWindow(s_Window);

    auto frameEnd = std::chrono::system_clock::now();
    s_DeltaTime = std::chrono::duration<float>(frameEnd - s_FrameStart).count();

    if (!SDL_GL_GetSwapInterval(nullptr)) {
        float targetTime = 1.0f / 144.0f;
        if (s_DeltaTime < targetTime) {
            float delay = targetTime - s_DeltaTime;
            SDL_Delay(static_cast<uint32_t>(delay * 1000.0f));
            frameEnd = std::chrono::system_clock::now();
            s_DeltaTime = std::chrono::duration<float>(frameEnd - s_FrameStart).count();
        }
    }

    if (s_DeltaTime > 0) s_CurrentFPS = static_cast<int>(1.0f / s_DeltaTime);
}

float GetDeltaTime() { return s_DeltaTime; }
int GetFPS() { return s_CurrentFPS; }
int GetWindowWidth() { return s_Width; }
int GetWindowHeight() { return s_Height; }

bool WindowShouldClose() {
    return !s_Running;
}

void CloseWindow() {
    Log::Info("Shutting down window and OpenGL context");
    ShutdownImGui();
    if (s_GLContext) SDL_GL_DestroyContext(s_GLContext);
    if (s_Window) SDL_DestroyWindow(s_Window);
    SDL_Quit();
    Log::Info("VOID Engine shut down.");
}