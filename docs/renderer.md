# Renderer

The Renderer is the main rendering interface exposed to the game. It does **not**
issue immediate draw calls itself — instead it forwards geometry to the Batch
Renderer, which is the heart of the graphics pipeline.

Responsibilities: scene management, camera handling, quad submission, text
rendering, particle rendering, tilemap rendering, and runtime statistics.

## Batch renderer

Rather than drawing every sprite immediately, geometry is accumulated into large
GPU buffers and flushed automatically when necessary.

| Resource | Limit |
|----------|------:|
| Quads | 100,000 |
| Vertices | 400,000 |
| Indices | 600,000 |
| Texture Slots | 32* |

\* Hardware dependent.

## Scenes

```cpp
Renderer::BeginScene(camera);
// ... draw calls ...
Renderer::EndScene();
```

## Drawing textured quads

```cpp
Renderer::DrawQuad(texture, {100, 100}, {64, 64});
```

With rotation:

```cpp
Renderer::DrawQuad(texture, {100, 100}, {64, 64}, 45.0f);
```

With color tint:

```cpp
Renderer::DrawQuad(texture, {100, 100}, {64, 64}, 0.0f, {1, 0, 0, 1});
```

With a custom pivot:

```cpp
Renderer::DrawQuad(texture, position, size, rotation, color, {0.5f, 0.5f});
```

With flipping:

```cpp
Renderer::DrawQuad(
    texture, position, size, 0.0f,
    {1, 1, 1, 1}, {0.5f, 0.5f},
    true, false
);
```

## Drawing colored quads

Solid rectangles can be drawn without creating a texture — internally the renderer
uses a built-in 1x1 white texture.

```cpp
Renderer::DrawColorQuad({200, 100}, {80, 80}, {0, 1, 0, 1});
```

With rotation:

```cpp
Renderer::DrawColorQuad(position, size, color, 30.0f);
```

## Statistics

```cpp
RendererStats stats = Renderer::GetStats();

std::cout << stats.DrawCalls << std::endl;
std::cout << stats.QuadCount << std::endl;
```