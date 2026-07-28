# Fonts

Fonts are generated into bitmap atlases during loading using **stb_truetype**,
with glyphs cached for high-performance text rendering.

## Loading

```cpp
auto font = AssetManager::LoadFont(
    "main",
    "assets/font.ttf",
    32.0f
);
```

## Rendering text

```cpp
Renderer::DrawText(*font, "Hello World", {100, 100});
```

With scale:

```cpp
Renderer::DrawText(*font, "Hello", {100, 100}, 2.0f);
```

With color:

```cpp
Renderer::DrawText(*font, "VOID Engine", {100, 100}, 1.0f, {1, 0, 0, 1});
```
