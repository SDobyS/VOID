#include "Window.h"
#include "ImGuiLayer.h"
#include "utils/Log.h"
#include "utils/SystemInfo.h"
#include "graphics/Renderer.h"
#include "AssetManager.h"
#include "Input.h"

#include <SDL3/SDL.h>
#include <glad/gl.h>
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif

namespace voidx {
    static SDL_Window* s_Window = nullptr;
    static SDL_GLContext s_GLContext = nullptr;
    static bool s_Running = true;
    static int s_Width = 0;
    static int s_Height = 0;

    static int s_FPSLimit = 0;
    static VSyncMode s_VSyncMode = VSyncMode::Disabled;
    static bool s_ShowImGui = true;
    static bool s_ShowDebugImGui = false;
    static bool s_Fullscreen = false;

    static std::chrono::steady_clock::time_point s_FrameStart;
    static std::chrono::steady_clock::time_point s_LastFrameTime;
    static float s_DeltaTime = 0.0f;
    static float s_FrameTimeMs = 0.0f;

    static float s_FPSTimer = 0.0f;
    static int s_FrameCounter = 0;
    static int s_CurrentFPS = 0;

    static void CleanupCore() {
        ShutdownImGui();
        Renderer::Shutdown();
        AssetManager::Shutdown();
        if (s_GLContext) { SDL_GL_DestroyContext(s_GLContext); s_GLContext = nullptr; }
        if (s_Window) { SDL_DestroyWindow(s_Window); s_Window = nullptr; }
        SDL_Quit();
    }

    bool InitWindow(const WindowConfig& config) {
        Log::Info("Starting VOID Engine...");
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            Log::Error("Window", std::string("SDL init failed: ") + SDL_GetError());
            return false;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, config.glMajor);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, config.glMinor);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, config.depthBits);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, config.stencilBits);

        if (config.samples > 0) {
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, config.samples);
        }

        Uint64 flags = SDL_WINDOW_OPENGL;
        if (config.resizable) flags |= SDL_WINDOW_RESIZABLE;
        if (config.borderless) flags |= SDL_WINDOW_BORDERLESS;
        if (config.fullscreen) flags |= SDL_WINDOW_FULLSCREEN;
        if (config.highDPI) flags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;

        int posX = config.x < 0 ? SDL_WINDOWPOS_CENTERED : config.x;
        int posY = config.y < 0 ? SDL_WINDOWPOS_CENTERED : config.y;

        s_Window = SDL_CreateWindow(config.title, config.width, config.height, flags);
        if (!s_Window) {
            Log::Error("Window", std::string("Window creation failed: ") + SDL_GetError());
            SDL_Quit();
            return false;
        }

        SDL_SetWindowPosition(s_Window, posX, posY);

        if (config.minWidth > 0 && config.minHeight > 0)
            SDL_SetWindowMinimumSize(s_Window, config.minWidth, config.minHeight);
        if (config.maxWidth > 0 && config.maxHeight > 0)
            SDL_SetWindowMaximumSize(s_Window, config.maxWidth, config.maxHeight);

        SDL_DisplayID display = SDL_GetDisplayForWindow(s_Window);
        if (const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display); mode) {
            Log::Info("Display", "Monitor refresh rate: " + std::to_string(mode->refresh_rate) + " Hz");
        }

        s_Width = config.width;
        s_Height = config.height;
        s_VSyncMode = config.vsync;
        s_ShowDebugImGui = config.showDebugUI;
        s_Fullscreen = config.fullscreen;

        if (s_VSyncMode != VSyncMode::Disabled && config.fpsLimit > 0) {
            Log::Error("Config", "VSync and FPS Limit cannot be enabled simultaneously. Disabling FPS Limit.");
            s_FPSLimit = 0;
        } else {
            s_FPSLimit = config.fpsLimit;
        }

        if (config.iconPath) {
            if (SDL_Surface* icon = SDL_LoadBMP(config.iconPath); icon) {
                SDL_SetWindowIcon(s_Window, icon);
                SDL_DestroySurface(icon);
            }
        }

        s_GLContext = SDL_GL_CreateContext(s_Window);
        if (!s_GLContext) {
            Log::Error("Window", std::string("GL Context failed: ") + SDL_GetError());
            SDL_DestroyWindow(s_Window);
            SDL_Quit();
            return false;
        }
        SDL_GL_MakeCurrent(s_Window, s_GLContext);

        if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
            Log::Error("Window", "GLAD init failed");
            CleanupCore();
            return false;
        }

        int glMajor, glMinor;
        glGetIntegerv(GL_MAJOR_VERSION, &glMajor);
        glGetIntegerv(GL_MINOR_VERSION, &glMinor);
        if (glMajor < config.glMajor || (glMajor == config.glMajor && glMinor < config.glMinor)) {
            Log::Error("Window", "Requested OpenGL " + std::to_string(config.glMajor) + "." + std::to_string(config.glMinor) + " but got " + std::to_string(glMajor) + "." + std::to_string(glMinor));
            CleanupCore();
            return false;
        }

        Log::Info("Window", "OpenGL renderer: " + std::string((char*)glGetString(GL_RENDERER)));

        if (s_VSyncMode == VSyncMode::Adaptive) {
            if (!SDL_GL_SetSwapInterval(-1)) {
                Log::Warning("VSync", "Adaptive VSync unavailable, using normal VSync");
                s_VSyncMode = VSyncMode::Enabled;
                SDL_GL_SetSwapInterval(1);
            }
        }
        if (s_VSyncMode == VSyncMode::Enabled) {
            if (!SDL_GL_SetSwapInterval(1)) {
                Log::Warning("VSync", "Failed to enable VSync");
                s_VSyncMode = VSyncMode::Disabled;
            }
        }
        if (s_VSyncMode == VSyncMode::Disabled) { SDL_GL_SetSwapInterval(0); }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (config.samples > 0) { glEnable(GL_MULTISAMPLE); }

        SystemInfo::PrintHardwareInfo();
        SystemInfo::PrintGPUInfo();

        AssetManager::Init();
        if (!Renderer::Init()) {
            Log::Error("Window", "Renderer initialization failed!");
            CleanupCore();
            return false;
        }

        if (!InitImGui(s_Window, s_GLContext)) {
            Log::Error("Window", "ImGui initialization failed!");
            CleanupCore();
            return false;
        }

        Log::Success("Window", "VOID Engine started successfully");
        s_LastFrameTime = std::chrono::steady_clock::now();
        return true;
    }

    void BeginDrawing() {
        s_FrameStart = std::chrono::steady_clock::now();
        s_DeltaTime = std::chrono::duration<float>(s_FrameStart - s_LastFrameTime).count();
        s_LastFrameTime = s_FrameStart;
        if (s_DeltaTime > 0.1f) { s_DeltaTime = 0.1f; }

        int drawableW, drawableH;
        SDL_GetWindowSizeInPixels(s_Window, &drawableW, &drawableH);
        s_Width = drawableW;
        s_Height = drawableH;

        glViewport(0, 0, drawableW, drawableH);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) s_Running = false;
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.scancode == SDL_SCANCODE_F11) ToggleFullscreen();
                if (event.key.scancode == SDL_SCANCODE_F8) s_ShowDebugImGui = !s_ShowDebugImGui;
            }
        }
        BeginImGui();
    }

    void EndDrawing() {
        if (s_ShowImGui) EndImGui();
        else ImGui::EndFrame();

        SDL_GL_SwapWindow(s_Window);

        #ifdef _WIN32
        if (s_VSyncMode != VSyncMode::Disabled) DwmFlush();
        #endif

        auto frameEnd = std::chrono::steady_clock::now();
        float frameTime = std::chrono::duration<float>(frameEnd - s_FrameStart).count();
        s_FrameTimeMs = frameTime * 1000.0f;

        if (s_VSyncMode == VSyncMode::Disabled && s_FPSLimit > 0) {
            float targetTime = 1.0f / static_cast<float>(s_FPSLimit);
            while (frameTime < targetTime) {
                SDL_Delay(1);
                frameEnd = std::chrono::steady_clock::now();
                frameTime = std::chrono::duration<float>(frameEnd - s_FrameStart).count();
                s_FrameTimeMs = frameTime * 1000.0f;
            }
        }

        s_FrameCounter++;
        s_FPSTimer += s_DeltaTime;
        if (s_FPSTimer >= 1.0f) {
            s_CurrentFPS = static_cast<int>(static_cast<float>(s_FrameCounter) / s_FPSTimer);
            s_FrameCounter = 0;
            s_FPSTimer = 0.0f;
        }
        Input::Update();
    }

    void SetWindowSize(int width, int height) {
        SDL_SetWindowSize(s_Window, width, height);
        s_Width = width; s_Height = height;
    }

    void ToggleFullscreen() {
        s_Fullscreen = !s_Fullscreen;
        SDL_SetWindowFullscreen(s_Window, s_Fullscreen);
        SDL_GL_MakeCurrent(s_Window, s_GLContext);
        if (s_VSyncMode == VSyncMode::Adaptive) SDL_GL_SetSwapInterval(-1);
        else if (s_VSyncMode == VSyncMode::Enabled) SDL_GL_SetSwapInterval(1);
        else SDL_GL_SetSwapInterval(0);
    }

    bool IsDebugUIVisible() { return s_ShowImGui && s_ShowDebugImGui; }
    float GetDeltaTime() { return s_DeltaTime; }
    float GetFrameTime() { return s_FrameTimeMs; }
    int GetFPS() { return s_CurrentFPS; }
    int GetWindowWidth() { return s_Width; }
    int GetWindowHeight() { return s_Height; }
    bool WindowShouldClose() { return !s_Running; }

    void CloseWindow() {
        Log::Info("Window", "Shutting down VOID Engine");
        CleanupCore();
        Log::Info("Window", "VOID Engine shut down cleanly.");
    }
}