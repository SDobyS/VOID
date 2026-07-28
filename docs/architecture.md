# Architecture

VOID is divided into independent modules, each responsible for a single subsystem.

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

Higher-level systems such as sprites, animations, tilemaps and particles are built
on top of the renderer instead of interacting with OpenGL directly. This keeps
responsibilities isolated while making the engine easier to extend and maintain.

## Frame lifecycle

A typical frame consists of the following stages:

1. Poll window events
2. Update the input system
3. Begin a new frame
4. Update game logic
5. Submit render commands
6. Flush the batch renderer
7. Present the rendered frame

## Core modules

| Module | Responsibility |
|--------|-----------------|
| [Window](window.md) | SDL3 init, GL context, fullscreen, VSync, monitor detection |
| [Input](input.md) | Keyboard / mouse state, fully hides SDL from game code |
| [Camera](camera.md) | Orthographic camera & view-projection matrix |
| [Asset Manager](assets.md) | Cached loading of textures, fonts and shaders |
| [Renderer](renderer.md) | Scene management, quad/text/particle/tilemap submission |
| Batch Renderer | Accumulates geometry into GPU buffers, auto-flushes |

## Resource management

All GPU resources follow RAII: they release their OpenGL handles automatically when
destroyed. The following classes are **move-only** (copy is disabled to prevent
accidental duplication of GPU resources):

- Texture
- Shader
- Font
- Mesh
- VertexArray
- Framebuffer

```cpp
Texture tex;
Texture another = std::move(tex);
```
