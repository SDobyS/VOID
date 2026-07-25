#include "Input.h"
#include <SDL3/SDL.h>

namespace Input {
    bool IsKeyDown(KeyCode key) {
        const bool* state = SDL_GetKeyboardState(nullptr);
        return state[static_cast<int>(key)];
    }
}