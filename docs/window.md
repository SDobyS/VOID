# Window

Responsible for SDL3 initialization, OpenGL context creation, window creation and
management, and swapping/presenting frames.

## Features

- SDL3 window management
- High-DPI support
- Fullscreen switching
- Window resizing
- Configurable OpenGL context
- Multi-monitor awareness
- MSAA support
- VSync configuration (including adaptive VSync)

## Initialization

```cpp
WindowConfig config;

config.title = "VOID";
config.width = 1280;
config.height = 720;
config.vsync = VSyncMode::Enabled;
config.showDebugUI = true;

if (!InitWindow(config))
    return 1;
```

## Shutdown

```cpp
CloseWindow();
```

## Hotkeys

| Key | Action |
|-----|--------|
| F8  | Toggle Dear ImGui debug UI |
| F11 | Toggle fullscreen |
