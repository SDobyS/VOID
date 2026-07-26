#include "ImGuiLayer.h"
#include "utils/Log.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

namespace voidx {
    bool InitImGui(SDL_Window* window, SDL_GLContext gl_context) {
        Log::Info("Initializing ImGui");
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        if (!ImGui_ImplSDL3_InitForOpenGL(window, gl_context)) {
            Log::Error("Failed to initialize ImGui SDL3 backend");
            return false;
        }
        if (!ImGui_ImplOpenGL3_Init("#version 460")) {
            Log::Error("Failed to initialize ImGui OpenGL3 backend");
            return false;
        }
        Log::Success("ImGui initialized successfully");
        return true;
    }

    void BeginImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    void EndImGui() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ShutdownImGui() {
        Log::Info("Shutting down ImGui");
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        Log::Success("ImGui shutdown complete");
    }
}