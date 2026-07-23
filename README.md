# VOID

<p align="center">

<img src="https://img.shields.io/badge/C%2B%2B-20-blue">
<img src="https://img.shields.io/badge/OpenGL-4.6-success">
<img src="https://img.shields.io/badge/SDL-3-orange">
<img src="https://img.shields.io/badge/GLAD-2-red">
<img src="https://img.shields.io/badge/CMake-3.25+-blueviolet">
<img src="https://img.shields.io/badge/vcpkg-supported-green">
<img src="https://img.shields.io/badge/License-MIT-yellow">

</p>

🇬🇧 English | 🇷🇺 [Русский](README_RU.md)

A lightweight game engine written in modern C++ using SDL3 and OpenGL.

The project is built from scratch to explore graphics programming, rendering techniques and engine architecture without relying on external game engines.

## Features

- SDL3 window management
- OpenGL 4.6 Core Profile
- GLAD 2 loader
- Modern shader pipeline
- External GLSL shader loading
- CMake build system
- vcpkg integration
- Cross-platform friendly architecture

## Requirements

- Visual Studio 2022+
- CMake 3.25+
- Ninja
- vcpkg

## Build

Configure:

```bash
cmake --preset windows-msvc-vcpkg
```

Build:

```bash
cmake --build build
```

## Example

<p align="center">
  <img src="docs/example.png" width="900">
</p>

_Currently renders the first triangle using the modern OpenGL pipeline._

## How it works

The engine creates an SDL3 window and initializes an OpenGL 4.6 Core Profile context.

GLAD loads OpenGL functions at runtime.

Shaders are loaded from external GLSL files, compiled, linked into a shader program and used to render geometry.

## Project Structure

```text
.
├── assets/
│   └── shaders/
│       ├── triangle.vert
│       └── triangle.frag
├── docs/
│   ├── cat.gif
│   └── example.png
├── src/
│   └── main.cpp
├── CMakeLists.txt
├── CMakePresets.json
├── LICENSE
├── README.md
└── README_RU.md
```

## Roadmap

- [x] SDL3 initialization
- [x] OpenGL context
- [x] GLAD 2 integration
- [x] Shader loading
- [x] First rendered triangle
- [ ] Vertex Buffer Objects (VBO)
- [ ] Vertex Array Objects (VAO)
- [ ] Element Buffer Objects (EBO)
- [ ] Texture loading
- [ ] Camera
- [ ] Model loading
- [ ] Renderer abstraction

## About

VOID is a personal graphics and engine development project focused on learning modern OpenGL and building an engine from the ground up.

The project emphasizes understanding the graphics pipeline rather than using existing rendering frameworks.

## License

MIT

---

<p align="center">
  <img src="docs/cat.gif" width="800">
</p>