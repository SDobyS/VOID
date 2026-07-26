#include "Input.h"
#include <SDL3/SDL.h>

namespace voidx {
    namespace {
        const bool* s_CurrentKeyState = nullptr;
        bool s_PrevKeyState[SDL_SCANCODE_COUNT] = {false};
    }

    namespace Input {
        void Update() {
            if (s_CurrentKeyState) {
                memcpy(s_PrevKeyState, s_CurrentKeyState, sizeof(s_PrevKeyState));
            }
        }

        bool IsKeyDown(KeyCode key) {
            s_CurrentKeyState = SDL_GetKeyboardState(nullptr);
            return s_CurrentKeyState[static_cast<int>(key)];
        }

        bool IsKeyPressed(KeyCode key) {
            if (!s_CurrentKeyState) s_CurrentKeyState = SDL_GetKeyboardState(nullptr);
            return s_CurrentKeyState[static_cast<int>(key)] && !s_PrevKeyState[static_cast<int>(key)];
        }

        bool IsKeyReleased(KeyCode key) {
            if (!s_CurrentKeyState) s_CurrentKeyState = SDL_GetKeyboardState(nullptr);
            return !s_CurrentKeyState[static_cast<int>(key)] && s_PrevKeyState[static_cast<int>(key)];
        }

        bool IsMouseButtonDown(MouseButton button) {
            return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_MASK(static_cast<int>(button));
        }

        glm::vec2 GetMousePosition() {
            float x, y;
            SDL_GetMouseState(&x, &y);
            return {x, y};
        }

        float GetMouseX() {
            float x, y; SDL_GetMouseState(&x, &y); return x;
        }

        float GetMouseY() {
            float x, y; SDL_GetMouseState(&x, &y); return y;
        }
    }
}