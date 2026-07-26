# VOID

<p align="center">

<img src="https://img.shields.io/badge/C%2B%2B-20-blue">
<img src="https://img.shields.io/badge/OpenGL-4.6-success">
<img src="https://img.shields.io/badge/SDL-3-orange">
<img src="https://img.shields.io/badge/GLAD-2-red">
<img src="https://img.shields.io/badge/GLM-Latest-green">
<img src="https://img.shields.io/badge/ImGui-Latest-purple">
<img src="https://img.shields.io/badge/CMake-3.25+-blueviolet">
<img src="https://img.shields.io/badge/License-MIT-yellow">

</p>

<p align="center">
A lightweight 2D game engine written in modern C++20 using SDL3 and OpenGL.
</p>

<p align="center">
Built from scratch to explore graphics programming, engine architecture and modern rendering techniques without relying on existing game engines.
</p>

---

## Overview

VOID is a lightweight 2D game engine focused on simplicity, performance and modern C++ design.

Instead of exposing SDL or OpenGL directly, the engine provides a clean, Raylib-inspired API while internally using a modular architecture based on RAII, automatic resource management and a high-performance batch renderer.

The project is primarily educational, but every subsystem is implemented as if it were part of a production-ready engine. The goal is not only to render sprites, but also to understand every stage of a modern rendering pipeline—from window creation to GPU resource management.

---

## ✨ Features

### Rendering

- High-performance **Batch Renderer**
- Automatic texture batching
- Hardware-accelerated OpenGL 4.6 renderer
- Orthographic camera
- Sprite rendering
- Animated sprites
- Colored quad rendering
- Rotation, scaling and custom pivot support
- Horizontal and vertical sprite flipping
- Framebuffer (Render Texture) support
- Runtime renderer statistics

### Graphics

- GLSL shader system
- Automatic shader loading
- Texture abstraction
- Mesh abstraction
- Vertex Array abstraction
- Bitmap font rendering
- Tilemap rendering (Tiled JSON)
- Particle system
- Automatic texture fallback on loading failure

### Engine

- Raylib-inspired global API
- Modular engine architecture
- Asset Manager with automatic caching
- RAII resource management
- Move-only GPU resources
- Automatic lifetime management
- Runtime debug interface powered by Dear ImGui

### Window

- SDL3 window management
- High-DPI support
- Fullscreen switching
- Window resizing
- Configurable OpenGL context
- Multi-monitor awareness
- MSAA support
- VSync configuration
- Adaptive VSync support

### Input

- Keyboard input
- Mouse input
- Frame-based key events
- SDL completely hidden from the game layer

### Utilities

- Delta time
- FPS counter
- Frame timing
- Colored logging
- GPU information
- System information
- Automatic dependency management with CMake FetchContent

---

## ⚡ Performance

The renderer is designed around dynamic batching in order to minimize OpenGL draw calls.

Current renderer limits per batch:

| Resource | Limit |
|----------|-------:|
| Quads | 20,000 |
| Vertices | 80,000 |
| Indices | 120,000 |
| Texture Slots | 32* |

\* Hardware dependent.

Instead of issuing one draw call for every sprite, geometry is accumulated into large GPU buffers and rendered in as few draw calls as possible.

This significantly reduces CPU overhead and improves rendering performance when drawing thousands of sprites.

---

## 🛠 Requirements

- Visual Studio 2022 or newer
- CMake 3.25+
- Git
- Python 3.x

---

## 🚀 Build

Clone the repository:

```bash
git clone https://github.com/SDobyS/VOID.git
cd VOID
```

Configure the project:

```bash
cmake --preset windows-msvc-debug
```

Build:

```bash
cmake --build build
```

All third-party dependencies are downloaded automatically using **CMake FetchContent**.

No external package manager such as **vcpkg** is required.

---

## 🚀 Quick Example

Game code only needs a single include:

```cpp
#include "void.h"
```

Creating a window:

```cpp
WindowConfig config;

config.title = "VOID";
config.width = 1280;
config.height = 720;
config.vsync = VSyncMode::Enabled;

if (!InitWindow(config))
    return 1;
```

Main loop:

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

The game layer never directly interacts with SDL3, OpenGL or GLAD.

The engine is responsible for managing the graphics API, GPU resources and rendering pipeline.

---

# ⚙️ How It Works

VOID provides a simple, Raylib-inspired API while internally using a modern C++20 architecture built around RAII, modular design and hardware-accelerated rendering.

Instead of exposing SDL3 or OpenGL directly, the engine hides platform-specific implementation details behind lightweight engine interfaces. Game code remains clean, portable and focused only on gameplay.

A typical frame consists of the following stages:

1. Poll window events
2. Update the input system
3. Begin a new frame
4. Update game logic
5. Submit render commands
6. Flush the batch renderer
7. Present the rendered frame

Internally, rendering is fully GPU accelerated through OpenGL 4.6.

---

# 🏗 Engine Architecture

The engine is divided into independent modules, each responsible for a single subsystem.

```
Application
      │
      ▼
 Window System
      │
      ▼
 Input System
      │
      ▼
 Renderer
      │
      ▼
 Batch Renderer
      │
      ▼
 OpenGL
```

Higher-level systems such as sprites, animations, tilemaps and particles are built on top of the renderer instead of interacting with OpenGL directly.

This architecture keeps responsibilities isolated while making the engine easier to extend and maintain.

---

# 📦 Core Modules

## Window

Responsible for:

- SDL3 initialization
- OpenGL context creation
- Window creation
- Fullscreen switching
- High-DPI support
- VSync
- Monitor detection
- Swap buffers
- Event polling

---

## Renderer

The Renderer is the main rendering interface exposed to the game.

Responsibilities include:

- Scene management
- Camera handling
- Quad submission
- Text rendering
- Particle rendering
- Tilemap rendering
- Renderer statistics

The renderer itself does not issue immediate draw calls.

Instead, it forwards geometry to the Batch Renderer.

---

## Batch Renderer

The Batch Renderer is the heart of the graphics pipeline.

Rather than drawing every sprite immediately, geometry is accumulated into large GPU buffers.

When necessary, the renderer automatically flushes the current batch.

Advantages include:

- Thousands of sprites per draw call
- Reduced CPU overhead
- Automatic texture slot management
- Dynamic vertex buffers
- Automatic batch flushing

Current limits:

- 20,000 quads
- 80,000 vertices
- 120,000 indices
- Up to 32 texture slots

---

## Asset Manager

Loading the same texture or shader multiple times wastes both memory and loading time.

The Asset Manager automatically caches resources.

Supported assets:

- Texture
- Shader
- Font

Internally resources are stored using `std::shared_ptr`, allowing safe sharing across multiple engine systems.

---

## Sprite System

Sprites are lightweight rendering objects that encapsulate:

- Texture
- Position
- Size
- Rotation
- Pivot
- Color tint
- Flip state

Sprites are submitted directly to the Batch Renderer.

---

## Animation System

Animations simplify sprite sheet playback.

Features include:

- Frame-based animation
- Variable playback speed
- Arbitrary frame count
- Configurable frame size
- Multiple rows
- Automatic UV generation

AnimatedSprite combines Sprite and Animation into a single high-level object.

---

## Font System

Fonts are generated into bitmap atlases during loading using **stb_truetype**.

Features:

- UTF-ready rendering pipeline
- Bitmap atlas generation
- Cached glyphs
- Scaling
- Color tint
- High-performance text rendering

---

## Tilemap System

Tilemaps are loaded directly from **Tiled JSON** maps.

Supported:

- CSV layers
- Embedded tilesets
- Multiple visible layers

Current limitations:

- External TSX files are not yet supported.
- Infinite maps are not supported.

---

## Particle System

The particle system uses object pooling.

Instead of allocating particles every frame, inactive particles are reused.

Benefits:

- No runtime allocations
- Stable performance
- Predictable memory usage

Supported particle properties include:

- Lifetime
- Velocity
- Gravity
- Rotation
- Scale
- Color interpolation

---

## Framebuffer

Framebuffers allow rendering into textures instead of directly to the screen.

Typical use cases:

- Render-to-texture
- Post-processing
- Off-screen rendering
- Mini-maps
- Lighting effects

---

## Input System

The Input module completely hides SDL from the public API.

Supported functionality:

- Keyboard state
- Mouse state
- Key pressed
- Key released
- Mouse position
- Mouse buttons

Input is updated once every frame.

---

## Dear ImGui Integration

VOID includes an optional Dear ImGui debug layer.

It can be enabled during window creation and toggled at runtime.

The debug interface is useful for:

- Renderer statistics
- FPS monitoring
- Camera debugging
- Runtime inspection
- Development tools

---

# 🧩 Resource Management

All GPU resources follow RAII principles.

Objects automatically release their OpenGL resources when destroyed.

The following classes are move-only:

- Texture
- Shader
- Mesh
- VertexArray
- Font
- Framebuffer

Copying is intentionally disabled in order to prevent accidental duplication of GPU resources.

This design makes ownership explicit while avoiding memory leaks.

---

## 📜 License

**MIT**

---

<p align="center">
  <img src="docs/cat.gif" width="800">
</p>


