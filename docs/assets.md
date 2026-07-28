# Asset Manager

Loading the same texture or shader multiple times wastes both memory and loading
time — the Asset Manager caches resources automatically. Internally, resources are
stored using `std::shared_ptr`, allowing safe sharing across multiple engine
systems.

Supported asset types: **Texture**, **Font**, **Shader**.

## Loading textures

```cpp
auto texture = AssetManager::LoadTexture(
    "player",
    "assets/player.png"
);
```

With an explicit filter:

```cpp
auto texture = AssetManager::LoadTexture(
    "player",
    "assets/player.png",
    TextureFilter::Linear
);
```

Available filters: `TextureFilter::Nearest`, `TextureFilter::Linear`.

## Getting cached assets

```cpp
auto texture = AssetManager::GetTexture("player");
```

## Unloading

```cpp
AssetManager::UnloadTexture("player");
```

## Fonts

```cpp
auto font = AssetManager::LoadFont("main", "assets/font.ttf", 32.0f);
auto font2 = AssetManager::GetFont("main");
AssetManager::UnloadFont("main");
```

## Shaders

```cpp
auto shader = AssetManager::LoadShader(
    "default",
    "assets/default.vert",
    "assets/default.frag"
);

auto s = AssetManager::GetShader("default");
AssetManager::UnloadShader("default");
```

## Checking existence

```cpp
if (AssetManager::Exists("player"))
{
}
```
