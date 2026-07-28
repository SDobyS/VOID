# Quick Start

Game code only needs a single include:

```cpp
#include "void.h"
```

All engine code lives inside the `voidx` namespace. The main engine header exposes
`using namespace voidx;`, so game code can use engine classes without qualifying
them every time:

```cpp
Texture texture;
Renderer::BeginScene(camera);
```

instead of

```cpp
voidx::Texture texture;
voidx::Renderer::BeginScene(camera);
```

## Creating a window

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

## Main loop

```cpp
OrthographicCamera camera(
    0.0f,
    1280.0f,
    720.0f,
    0.0f
);

while (!WindowShouldClose())
{
    BeginDrawing();

    float dt = GetDeltaTime();

    Renderer::BeginScene(camera);

    // Draw your game here

    Renderer::EndScene();

    EndDrawing();
}

CloseWindow();
```

`BeginDrawing()` starts a new frame, `EndDrawing()` presents the rendered frame.
`Renderer::BeginScene()` prepares the batch renderer and `Renderer::EndScene()`
uploads the accumulated geometry to the GPU.

The game layer never directly interacts with SDL3, OpenGL or GLAD — the engine
manages the graphics API, GPU resources and rendering pipeline for you.

Next: [Architecture](architecture.md).
