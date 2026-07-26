# VOID Engine

A lightweight 2D game engine written in C++ using SDL3, OpenGL 4.6, GLM and Dear ImGui.

VOID Engine is designed around a simple API similar to raylib while keeping a modern C++ architecture with automatic resource management, batch rendering and hardware accelerated rendering.

---

# Features

- Modern OpenGL 4.6 renderer
- SDL3 window and input system
- Orthographic camera
- High performance batch renderer
- Sprite rendering
- Animated sprites
- Bitmap font rendering
- Tilemap support (Tiled JSON)
- Particle system with object pooling
- Asset manager with automatic caching
- Framebuffers
- Mesh rendering
- GLSL shader support
- Dear ImGui integration
- RAII resource management
- Move semantics for GPU resources

---

# Architecture

All engine code lives inside the `voidx` namespace.

The main engine header exposes

```cpp
using namespace voidx;
```

which allows game code to use engine classes without writing the namespace every time.

Example

```cpp
Texture texture;
Renderer::BeginScene(camera);
```

instead of

```cpp
voidx::Texture texture;
voidx::Renderer::BeginScene(camera);
```

---

# Resource Management

GPU resources are managed using RAII.

OpenGL objects are automatically destroyed when the owning object goes out of scope.

The following classes are **moveable** but **not copyable**:

- Texture
- Shader
- Font
- Mesh
- VertexArray
- Framebuffer

Example

```cpp
Texture tex;

Texture another = std::move(tex);
```

Copying is intentionally disabled.

---

# Creating a Window

Initialize the engine before using any rendering functionality.

```cpp
WindowConfig config;

config.title = "My Game";
config.width = 1280;
config.height = 720;
config.vsync = VSyncMode::Enabled;
config.showDebugUI = true;

if (!InitWindow(config))
    return 1;
```

Shutdown the engine before exiting.

```cpp
CloseWindow();
```

---

# Main Loop

The engine follows a simple frame loop.

```cpp
while (!WindowShouldClose())
{
    BeginDrawing();

    float dt = GetDeltaTime();

    // Update game

    Renderer::BeginScene(camera);

    // Draw game

    Renderer::EndScene();

    EndDrawing();
}
```

`BeginDrawing()` starts a new frame.

`EndDrawing()` presents the rendered frame.

`Renderer::BeginScene()` prepares the batch renderer.

`Renderer::EndScene()` uploads accumulated geometry to the GPU and performs rendering.

---

# Engine Hotkeys

The engine provides several built-in shortcuts.

| Key | Action |
|------|--------|
| F8 | Toggle Dear ImGui Debug UI |
| F11 | Toggle Fullscreen |

---

# Orthographic Camera

The renderer uses an orthographic camera.

```cpp
OrthographicCamera camera(
    0.0f,
    1280.0f,
    720.0f,
    0.0f
);
```

Move the camera.

```cpp
camera.SetPosition({200.0f, 100.0f, 0.0f});
```

Rotate the camera.

```cpp
camera.SetRotation(45.0f);
```

Zoom.

```cpp
camera.SetZoom(2.0f);
```

The renderer automatically uses the camera's ViewProjection matrix.

```cpp
Renderer::BeginScene(camera);
```

---

# Batch Renderer

The renderer is optimized around batching.

Instead of issuing one draw call for every sprite, geometry is accumulated into a large vertex buffer.

When the frame ends, all accumulated quads are rendered using a minimal number of OpenGL draw calls.

Current limits

- Up to **20,000 quads** per batch
- Up to **80,000 vertices**
- Up to **120,000 indices**
- Up to **32 texture slots** (hardware dependent)

This dramatically reduces CPU overhead when rendering large numbers of sprites.

---

# Drawing Textured Quads

Draw a texture.

```cpp
Renderer::DrawQuad(
    texture,
    {100, 100},
    {64, 64}
);
```

With rotation.

```cpp
Renderer::DrawQuad(
    texture,
    {100,100},
    {64,64},
    45.0f
);
```

With color tint.

```cpp
Renderer::DrawQuad(
    texture,
    {100,100},
    {64,64},
    0.0f,
    {1,0,0,1}
);
```

Custom pivot.

```cpp
Renderer::DrawQuad(
    texture,
    position,
    size,
    rotation,
    color,
    {0.5f,0.5f}
);
```

Flip horizontally.

```cpp
Renderer::DrawQuad(
    texture,
    position,
    size,
    0.0f,
    {1,1,1,1},
    {0.5f,0.5f},
    true,
    false
);
```

---

# Drawing Colored Quads

The renderer can draw solid colored rectangles without creating textures.

```cpp
Renderer::DrawColorQuad(
    {200,100},
    {80,80},
    {0,1,0,1}
);
```

Rotation is also supported.

```cpp
Renderer::DrawColorQuad(
    position,
    size,
    color,
    30.0f
);
```

Internally the renderer uses a built-in 1×1 white texture.

--- 

# Sprite

`Sprite` is a lightweight wrapper around a texture that simplifies rendering.

It stores its own transform, color and rendering properties.

```cpp
auto texture = AssetManager::LoadTexture(
    "player",
    "assets/player.png"
);

Sprite player(
    texture,
    {100,100},
    {64,64}
);
```

Draw the sprite.

```cpp
player.Draw();
```

---

## Position

```cpp
player.SetPosition({300,200});
```

---

## Rotation

Rotation is specified in degrees.

```cpp
player.SetRotation(45.0f);
```

---

## Pivot

The pivot controls the rotation origin.

Default pivot

```cpp
{0.5f,0.5f}
```

(center)

Custom pivot

```cpp
player.SetPivot({0.0f,0.0f});
```

(top-left corner)

---

## Flip

Horizontal

```cpp
player.SetFlipX(true);
```

Vertical

```cpp
player.SetFlipY(true);
```

---

## Color Tint

Sprites can be tinted.

```cpp
player.SetColor({
    1.0f,
    0.5f,
    0.5f,
    1.0f
});
```

White keeps the original texture.

```cpp
{1,1,1,1}
```

---

# Animation

`Animation` is responsible for sprite sheet playback.

It stores

- current frame
- frame size
- animation speed
- frame count
- frames per row

Example

```cpp
Animation animation(
    texture,
    64,
    64,
    8,
    0.1f,
    4
);
```

Arguments

| Parameter | Description |
|-----------|-------------|
| texture | Sprite sheet |
| frameWidth | Width of one frame |
| frameHeight | Height of one frame |
| frameCount | Total frame count |
| frameTime | Seconds per frame |
| framesPerRow | Frames contained in one row |

Update every frame.

```cpp
animation.Update(dt);
```

Change playback speed.

```cpp
animation.SetFrameTime(0.05f);
```

---

# AnimatedSprite

`AnimatedSprite` combines `Sprite` and `Animation`.

Instead of calculating UV coordinates manually, the current animation frame is rendered automatically.

Example

```cpp
AnimatedSprite player(
    texture,
    64,
    64,
    8,
    0.1f,
    4
);
```

Move the sprite.

```cpp
player.SetPosition({200,300});
```

Rotate.

```cpp
player.SetRotation(20.0f);
```

Flip.

```cpp
player.SetFlipX(true);
```

Color.

```cpp
player.SetColor({
    1,
    1,
    1,
    1
});
```

Update.

```cpp
player.Update(dt);
```

Draw.

```cpp
player.Draw();
```

---

# Asset Manager

The Asset Manager caches resources and prevents loading the same asset multiple times.

Internally all resources are stored using `std::shared_ptr`.

Supported asset types

- Texture
- Font
- Shader

---

## Loading Textures

```cpp
auto texture = AssetManager::LoadTexture(
    "player",
    "assets/player.png"
);
```

Specify filtering.

```cpp
auto texture = AssetManager::LoadTexture(
    "player",
    "assets/player.png",
    TextureFilter::Linear
);
```

Available filters

```cpp
TextureFilter::Nearest
TextureFilter::Linear
```

---

## Getting Cached Assets

```cpp
auto texture =
    AssetManager::GetTexture("player");
```

---

## Unloading

```cpp
AssetManager::UnloadTexture("player");
```

---

## Fonts

```cpp
auto font =
    AssetManager::LoadFont(
        "main",
        "assets/font.ttf",
        32.0f
    );
```

Retrieve later.

```cpp
auto font =
    AssetManager::GetFont("main");
```

Unload.

```cpp
AssetManager::UnloadFont("main");
```

---

## Shaders

```cpp
auto shader =
AssetManager::LoadShader(
    "default",
    "assets/default.vert",
    "assets/default.frag"
);
```

Retrieve.

```cpp
auto shader =
AssetManager::GetShader("default");
```

Unload.

```cpp
AssetManager::UnloadShader("default");
```

---

## Exists

Check whether an asset is already loaded.

```cpp
if (AssetManager::Exists("player"))
{
}
```

---

# Font Rendering

Fonts are generated into bitmap atlases during loading.

Load.

```cpp
auto font =
AssetManager::LoadFont(
    "main",
    "assets/font.ttf",
    32.0f
);
```

Render text.

```cpp
Renderer::DrawText(
    *font,
    "Hello World",
    {100,100}
);
```

Scale.

```cpp
Renderer::DrawText(
    *font,
    "Hello",
    {100,100},
    2.0f
);
```

Color.

```cpp
Renderer::DrawText(
    *font,
    "VOID Engine",
    {100,100},
    1.0f,
    {1,0,0,1}
);
```

---

# Input

The engine provides frame-based keyboard and mouse input.

---

## Keyboard

Held key.

```cpp
if (Input::IsKeyDown(KEY_W))
{
}
```

Pressed during the current frame.

```cpp
if (Input::IsKeyPressed(KEY_SPACE))
{
}
```

Released during the current frame.

```cpp
if (Input::IsKeyReleased(KEY_ESCAPE))
{
}
```

---

## Mouse

Left button.

```cpp
if (Input::IsMouseButtonDown(
    MOUSE_BUTTON_LEFT
))
{
}
```

Mouse position.

```cpp
glm::vec2 mouse =
Input::GetMousePosition();
```

---

# Texture

Textures are GPU resources managed automatically.

Load directly.

```cpp
Texture texture;

texture.Load(
    "assets/player.png"
);
```

Bind.

```cpp
texture.Bind();
```

Bind to a specific slot.

```cpp
texture.Bind(3);
```

Unbind.

```cpp
texture.Unbind();
```

Retrieve texture size.

```cpp
int width = texture.GetWidth();

int height = texture.GetHeight();
```

Retrieve OpenGL handle.

```cpp
GLuint id =
texture.GetID();
```

Textures support move semantics but cannot be copied.

---

# Tilemaps

The engine supports tilemaps exported from **Tiled** in JSON format.

Only **tile layers** are rendered.

---

## Creating a Tilemap

Load the tileset texture.

```cpp
auto tileset =
AssetManager::LoadTexture(
    "tiles",
    "assets/tileset.png"
);
```

Create a tilemap.

```cpp
Tilemap map(
    tileset,
    32
);
```

The second parameter specifies the tile size in pixels.

You can also explicitly provide the number of columns in the tileset.

```cpp
Tilemap map(
    tileset,
    32,
    8
);
```

---

## Loading a Map

```cpp
if (!map.LoadFromTiledJSON(
    "assets/level.json"))
{
    return;
}
```

The loader automatically reads

- map width
- map height
- visible tile layers
- tile data
- tileset columns

---

## Drawing

Draw every visible layer.

```cpp
map.Draw();
```

Draw only one layer.

```cpp
map.DrawLayer(
    "Foreground"
);
```

---

## Requirements

The JSON map should be exported from **Tiled**.

Supported

- Embedded tilesets
- CSV tile layers
- Multiple visible layers

Unsupported

- External TSX tilesets
- Base64 encoded layers
- Infinite maps

---

# Particle System

The particle system uses an object pool.

Particles are reused instead of being allocated every frame.

This avoids runtime allocations and keeps performance stable.

---

## Creating a System

```cpp
ParticleSystem particles(1000);
```

The constructor specifies the maximum particle capacity.

---

## Configuration

```cpp
ParticleConfig config;
```

### Lifetime

```cpp
config.LifeTime = 1.5f;
```

---

### Speed

```cpp
config.SpeedMin = 50.0f;
config.SpeedMax = 200.0f;
```

---

### Size

```cpp
config.SizeStart = 16.0f;
config.SizeEnd = 0.0f;
```

---

### Colors

```cpp
config.ColorStart =
{
    1,
    1,
    0,
    1
};

config.ColorEnd =
{
    1,
    0,
    0,
    0
};
```

---

### Gravity

```cpp
config.Gravity =
{
    0,
    150
};
```

---

### Direction

```cpp
config.DirectionAngle = 0.0f;
config.SpreadAngle = glm::radians(45.0f);
```

---

### Rotation

```cpp
config.RotationSpeedMin = -90.0f;
config.RotationSpeedMax = 90.0f;
```

---

## Emitting Particles

```cpp
particles.Emit(
    {400,300},
    20,
    config
);
```

Arguments

| Parameter | Description |
|-----------|-------------|
| Position | Spawn position |
| Count | Number of particles |
| Config | Particle configuration |

---

## Updating

```cpp
particles.Update(dt);
```

---

## Rendering

```cpp
particles.Draw();
```

---

## Statistics

Current number of active particles.

```cpp
uint32_t active =
particles.GetActiveCount();
```

Maximum capacity.

```cpp
uint32_t capacity =
particles.GetCapacity();
```

---

# Framebuffer

Framebuffers allow rendering into textures.

Create one.

```cpp
Framebuffer framebuffer(
    1280,
    720
);
```

Bind.

```cpp
framebuffer.Bind();
```

Return to the default framebuffer.

```cpp
framebuffer.Unbind();
```

Resize.

```cpp
framebuffer.Resize(
    1920,
    1080
);
```

Retrieve the color attachment.

```cpp
GLuint texture =
framebuffer.GetTextureID();
```

---

# Mesh

A mesh stores custom geometry.

Create one.

```cpp
Mesh mesh;

mesh.Create(
    vertices,
    sizeof(vertices),
    indices,
    sizeof(indices)
);
```

Render.

```cpp
mesh.Draw();
```

Bind manually.

```cpp
mesh.Bind();
```

Meshes own their GPU buffers and automatically release them when destroyed.

---

# Shader

Shaders wrap OpenGL shader programs.

Load.

```cpp
Shader shader;

shader.Load(
    "default.vert",
    "default.frag"
);
```

Use.

```cpp
shader.Use();
```

---

## Uniforms

Integer.

```cpp
shader.SetUniform1i(
    "u_Texture",
    0
);
```

Integer array.

```cpp
shader.SetUniform1iv(
    "u_Textures",
    32,
    samplers
);
```

Matrix.

```cpp
shader.SetUniformMat4f(
    "u_ViewProj",
    matrix
);
```

Color.

```cpp
shader.SetUniform4f(
    "u_Color",
    {1,1,1,1}
);
```

---

# Renderer Statistics

The renderer exposes runtime statistics.

```cpp
RendererStats stats =
Renderer::GetStats();
```

Example.

```cpp
std::cout
<< stats.DrawCalls
<< std::endl;

std::cout
<< stats.QuadCount
<< std::endl;
```

These values are useful for debugging rendering performance.

---

# Dear ImGui

VOID Engine includes optional Dear ImGui integration.

The debug interface can be enabled when creating the window.

```cpp
config.showDebugUI = true;
```

Toggle the interface at runtime.

| Key | Action |
|------|--------|
| F8 | Toggle Debug UI |

---

# Performance

The renderer is designed around batching.

Features include

- Dynamic vertex buffers
- Texture slot batching
- Automatic batch flushing
- Quad rendering
- Single draw call for thousands of sprites
- Hardware accelerated rendering
- Cached shaders
- Cached textures

---

# Resource Lifetime

All GPU resources are automatically released.

Objects owning OpenGL resources cannot be copied.

Supported move-only classes

- Texture
- Shader
- Font
- Mesh
- VertexArray
- Framebuffer

This prevents accidental OpenGL resource duplication.

---

# Dependencies

VOID Engine uses the following libraries.

- SDL3
- OpenGL 4.6
- GLAD
- GLM
- Dear ImGui
- stb_image
- stb_truetype
- nlohmann/json

---

# License

See the project license for licensing information.