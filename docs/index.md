# VOID Engine

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue)
![OpenGL 4.6](https://img.shields.io/badge/OpenGL-4.6-success)
![SDL3](https://img.shields.io/badge/SDL-3-orange)
![GLAD2](https://img.shields.io/badge/GLAD-2-red)
![GLM](https://img.shields.io/badge/GLM-Latest-green)
![ImGui](https://img.shields.io/badge/ImGui-Latest-purple)
![CMake](https://img.shields.io/badge/CMake-3.25+-blueviolet)
![License MIT](https://img.shields.io/badge/License-MIT-yellow)

A lightweight 2D game engine written in modern C++20 using SDL3 and OpenGL.

Built from scratch to explore graphics programming, engine architecture and modern
rendering techniques without relying on existing game engines.

VOID is designed around a simple, **raylib-inspired** API while internally using a
modern C++ architecture with automatic resource management, batch rendering and
hardware-accelerated rendering.

## Why VOID?

The project is primarily educational, but every subsystem is implemented as if it
were part of a production-ready engine. The goal is not only to render sprites, but
to understand every stage of a modern rendering pipeline — from window creation to
GPU resource management.

## Feature Overview

- Hardware-accelerated OpenGL 4.6 batch renderer
- SDL3 window & input system, fully hidden from game code
- Orthographic camera
- Sprites, animated sprites, bitmap fonts
- Tiled JSON tilemap support
- Particle system with object pooling
- Asset manager with automatic caching
- Framebuffers (render-to-texture)
- RAII / move-only GPU resources
- Optional Dear ImGui debug layer

## Performance

| Resource | Limit |
|----------|------:|
| Quads | 100,000 |
| Vertices | 400,000 |
| Indices | 600,000 |
| Texture Slots | 32* |

\* Hardware dependent.

Geometry is accumulated into large GPU buffers and flushed in as few draw calls as
possible, instead of issuing one draw call per sprite.

## Where to start

- [Installation](getting-started.md)
- [Quick Start](quick-start.md)
- [Architecture](architecture.md)
- [API Reference](api.md)

## License

MIT — see the repository's `LICENSE` file.