#include "Input.h"
#include "Window.h"
#include <SDL3/SDL.h>
#include <cstring>

namespace voidx {
    namespace {
        const bool* s_CurrentKeyState = nullptr;
        bool s_PrevKeyState[SDL_SCANCODE_COUNT] = {false};

        Uint32 s_CurrentMouseState = 0;
        Uint32 s_PrevMouseState = 0;
        float s_MouseX = 0.0f;
        float s_MouseY = 0.0f;
        float s_MouseWheel = 0.0f;

        constexpr int MAX_GAMEPADS = 4;
        SDL_Gamepad* s_Gamepads[MAX_GAMEPADS] = {nullptr};
        bool s_CurrentGamepadButtons[MAX_GAMEPADS][15] = {false};
        bool s_PrevGamepadButtons[MAX_GAMEPADS][15] = {false};

        constexpr int MAX_TOUCH_POINTS = 8;
        struct TouchPoint {
            bool active = false;
            SDL_FingerID id = 0;
            float x = 0.0f;
            float y = 0.0f;
        };
        TouchPoint s_TouchPoints[MAX_TOUCH_POINTS];
        int s_TouchCount = 0;

        SDL_GamepadButton MapGamepadButton(GamepadButton button) {
            switch (button) {
                case GAMEPAD_BUTTON_A: return SDL_GAMEPAD_BUTTON_SOUTH;
                case GAMEPAD_BUTTON_B: return SDL_GAMEPAD_BUTTON_EAST;
                case GAMEPAD_BUTTON_X: return SDL_GAMEPAD_BUTTON_WEST;
                case GAMEPAD_BUTTON_Y: return SDL_GAMEPAD_BUTTON_NORTH;
                case GAMEPAD_BUTTON_BACK: return SDL_GAMEPAD_BUTTON_BACK;
                case GAMEPAD_BUTTON_GUIDE: return SDL_GAMEPAD_BUTTON_GUIDE;
                case GAMEPAD_BUTTON_START: return SDL_GAMEPAD_BUTTON_START;
                case GAMEPAD_BUTTON_LEFT_STICK: return SDL_GAMEPAD_BUTTON_LEFT_STICK;
                case GAMEPAD_BUTTON_RIGHT_STICK: return SDL_GAMEPAD_BUTTON_RIGHT_STICK;
                case GAMEPAD_BUTTON_LEFT_SHOULDER: return SDL_GAMEPAD_BUTTON_LEFT_SHOULDER;
                case GAMEPAD_BUTTON_RIGHT_SHOULDER: return SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER;
                case GAMEPAD_BUTTON_DPAD_UP: return SDL_GAMEPAD_BUTTON_DPAD_UP;
                case GAMEPAD_BUTTON_DPAD_DOWN: return SDL_GAMEPAD_BUTTON_DPAD_DOWN;
                case GAMEPAD_BUTTON_DPAD_LEFT: return SDL_GAMEPAD_BUTTON_DPAD_LEFT;
                case GAMEPAD_BUTTON_DPAD_RIGHT: return SDL_GAMEPAD_BUTTON_DPAD_RIGHT;
                default: return SDL_GAMEPAD_BUTTON_INVALID;
            }
        }

        SDL_GamepadAxis MapGamepadAxis(GamepadAxis axis) {
            switch (axis) {
                case GAMEPAD_AXIS_LEFT_X: return SDL_GAMEPAD_AXIS_LEFTX;
                case GAMEPAD_AXIS_LEFT_Y: return SDL_GAMEPAD_AXIS_LEFTY;
                case GAMEPAD_AXIS_RIGHT_X: return SDL_GAMEPAD_AXIS_RIGHTX;
                case GAMEPAD_AXIS_RIGHT_Y: return SDL_GAMEPAD_AXIS_RIGHTY;
                case GAMEPAD_AXIS_LEFT_TRIGGER: return SDL_GAMEPAD_AXIS_LEFT_TRIGGER;
                case GAMEPAD_AXIS_RIGHT_TRIGGER: return SDL_GAMEPAD_AXIS_RIGHT_TRIGGER;
                default: return SDL_GAMEPAD_AXIS_INVALID;
            }
        }
    }

    namespace Input {

    void ProcessEvent(const SDL_Event& event) {
        if (event.type == SDL_EVENT_MOUSE_WHEEL) {
            s_MouseWheel += event.wheel.y;
        } else if (event.type == SDL_EVENT_GAMEPAD_ADDED) {
            SDL_Gamepad* pad = SDL_OpenGamepad(event.gdevice.which);
            if (pad) {
                for (int i = 0; i < MAX_GAMEPADS; ++i) {
                    if (!s_Gamepads[i]) {
                        s_Gamepads[i] = pad;
                        break;
                    }
                }
            }
        } else if (event.type == SDL_EVENT_GAMEPAD_REMOVED) {
            for (int i = 0; i < MAX_GAMEPADS; ++i) {
                if (s_Gamepads[i] && SDL_GetGamepadID(s_Gamepads[i]) == event.gdevice.which) {
                    SDL_CloseGamepad(s_Gamepads[i]);
                    s_Gamepads[i] = nullptr;
                    for (int b = 0; b < 15; ++b) {
                        s_CurrentGamepadButtons[i][b] = false;
                        s_PrevGamepadButtons[i][b] = false;
                    }
                    break;
                }
            }
        } else if (event.type == SDL_EVENT_FINGER_DOWN) {
            for (int i = 0; i < MAX_TOUCH_POINTS; ++i) {
                if (!s_TouchPoints[i].active) {
                    s_TouchPoints[i].active = true;
                    s_TouchPoints[i].id = event.tfinger.fingerID;
                    s_TouchPoints[i].x = event.tfinger.x * GetWindowWidth();
                    s_TouchPoints[i].y = event.tfinger.y * GetWindowHeight();
                    s_TouchCount++;
                    break;
                }
            }
        } else if (event.type == SDL_EVENT_FINGER_MOTION) {
            for (int i = 0; i < MAX_TOUCH_POINTS; ++i) {
                if (s_TouchPoints[i].active && s_TouchPoints[i].id == event.tfinger.fingerID) {
                    s_TouchPoints[i].x = event.tfinger.x * GetWindowWidth();
                    s_TouchPoints[i].y = event.tfinger.y * GetWindowHeight();
                    break;
                }
            }
        } else if (event.type == SDL_EVENT_FINGER_UP) {
            for (int i = 0; i < MAX_TOUCH_POINTS; ++i) {
                if (s_TouchPoints[i].active && s_TouchPoints[i].id == event.tfinger.fingerID) {
                    s_TouchPoints[i].active = false;
                    s_TouchCount--;
                    break;
                }
            }
        }
    }

    void Update() {
        if (s_CurrentKeyState) {
            memcpy(s_PrevKeyState, s_CurrentKeyState, sizeof(s_PrevKeyState));
        }

        s_PrevMouseState = s_CurrentMouseState;
        s_CurrentMouseState = SDL_GetMouseState(&s_MouseX, &s_MouseY);

        s_MouseWheel = 0.0f;

        for (int i = 0; i < MAX_GAMEPADS; ++i) {
            if (s_Gamepads[i]) {
                for (int b = 0; b < 15; ++b) {
                    s_PrevGamepadButtons[i][b] = s_CurrentGamepadButtons[i][b];
                    s_CurrentGamepadButtons[i][b] = SDL_GetGamepadButton(s_Gamepads[i], static_cast<SDL_GamepadButton>(b));
                }
            }
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
        return s_CurrentMouseState & SDL_BUTTON_MASK(static_cast<int>(button));
    }

    bool IsMouseButtonPressed(MouseButton button) {
        return (s_CurrentMouseState & SDL_BUTTON_MASK(static_cast<int>(button))) && !(s_PrevMouseState & SDL_BUTTON_MASK(static_cast<int>(button)));
    }

    bool IsMouseButtonReleased(MouseButton button) {
        return !(s_CurrentMouseState & SDL_BUTTON_MASK(static_cast<int>(button))) && (s_PrevMouseState & SDL_BUTTON_MASK(static_cast<int>(button)));
    }

    glm::vec2 GetMousePosition() {
        return {s_MouseX, s_MouseY};
    }

    float GetMouseX() {
        return s_MouseX;
    }

    float GetMouseY() {
        return s_MouseY;
    }

    float GetMouseWheelMove() {
        return s_MouseWheel;
    }

    bool IsGamepadAvailable(int index) {
        if (index < 0 || index >= MAX_GAMEPADS) return false;
        return s_Gamepads[index] != nullptr;
    }

    float GetGamepadAxisMovement(int index, GamepadAxis axis) {
        if (index < 0 || index >= MAX_GAMEPADS || !s_Gamepads[index]) return 0.0f;
        SDL_GamepadAxis sdlAxis = MapGamepadAxis(axis);
        if (sdlAxis == SDL_GAMEPAD_AXIS_INVALID) return 0.0f;

        Sint16 value = SDL_GetGamepadAxis(s_Gamepads[index], sdlAxis);
        if (sdlAxis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER || sdlAxis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) {
            return static_cast<float>(value) / 32767.0f;
        }
        return static_cast<float>(value) / 32768.0f;
    }

    bool IsGamepadButtonDown(int index, GamepadButton button) {
        if (index < 0 || index >= MAX_GAMEPADS || !s_Gamepads[index]) return false;
        SDL_GamepadButton sdlButton = MapGamepadButton(button);
        if (sdlButton == SDL_GAMEPAD_BUTTON_INVALID) return false;
        return s_CurrentGamepadButtons[index][static_cast<int>(sdlButton)];
    }

    bool IsGamepadButtonPressed(int index, GamepadButton button) {
        if (index < 0 || index >= MAX_GAMEPADS || !s_Gamepads[index]) return false;
        SDL_GamepadButton sdlButton = MapGamepadButton(button);
        if (sdlButton == SDL_GAMEPAD_BUTTON_INVALID) return false;
        return s_CurrentGamepadButtons[index][static_cast<int>(sdlButton)] && !s_PrevGamepadButtons[index][static_cast<int>(sdlButton)];
    }

    bool IsGamepadButtonReleased(int index, GamepadButton button) {
        if (index < 0 || index >= MAX_GAMEPADS || !s_Gamepads[index]) return false;
        SDL_GamepadButton sdlButton = MapGamepadButton(button);
        if (sdlButton == SDL_GAMEPAD_BUTTON_INVALID) return false;
        return !s_CurrentGamepadButtons[index][static_cast<int>(sdlButton)] && s_PrevGamepadButtons[index][static_cast<int>(sdlButton)];
    }

    int GetTouchPointCount() {
        return s_TouchCount;
    }

    glm::vec2 GetTouchPosition(int index) {
        if (index < 0 || index >= MAX_TOUCH_POINTS || !s_TouchPoints[index].active) return {0.0f, 0.0f};
        return {s_TouchPoints[index].x, s_TouchPoints[index].y};
    }

    }
}