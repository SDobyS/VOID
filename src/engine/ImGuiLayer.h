#pragma once
#include <SDL3/SDL.h>

bool InitImGui(SDL_Window* window, SDL_GLContext gl_context);
void BeginImGui();
void EndImGui();
void ShutdownImGui();