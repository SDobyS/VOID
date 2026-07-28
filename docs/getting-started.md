# Installation

## Requirements

- Visual Studio 2022 or newer
- CMake 3.25+
- Git
- Python 3.x

All third-party dependencies (SDL3, GLAD, GLM, Dear ImGui, stb_image, stb_truetype,
nlohmann/json...) are downloaded automatically using **CMake FetchContent**.
No external package manager such as **vcpkg** is required.

## Clone

```bash
git clone https://github.com/SDobyS/VOID.git
cd VOID
```

## Configure

```bash
cmake --preset windows-msvc-debug
```

## Build

```bash
cmake --build build
```

Once the build finishes, continue with the [Quick Start](quick-start.md) guide.
