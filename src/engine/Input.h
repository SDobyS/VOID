#pragma once

enum KeyCode {
    KEY_UNKNOWN  = 0,
    KEY_A        = 4,
    KEY_D        = 7,
    KEY_S        = 22,
    KEY_W        = 26,
    KEY_ESCAPE   = 41,
    KEY_SPACE    = 44
};

namespace Input {
    bool IsKeyDown(KeyCode key);
}