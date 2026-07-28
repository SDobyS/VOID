# Sprite

`Sprite` is a lightweight wrapper around a texture that simplifies rendering. It
stores its own transform, color and rendering properties, and is submitted
directly to the Batch Renderer.

## Creating a sprite

```cpp
auto texture = AssetManager::LoadTexture("player", "assets/player.png");

Sprite player(texture, {100, 100}, {64, 64});
```

## Drawing

```cpp
player.Draw();
```

## Position

```cpp
player.SetPosition({300, 200});
```

## Rotation

Rotation is specified in degrees.

```cpp
player.SetRotation(45.0f);
```

## Pivot

Controls the rotation origin. Default is `{0.5f, 0.5f}` (center).

```cpp
player.SetPivot({0.0f, 0.0f}); // top-left corner
```

## Flip

```cpp
player.SetFlipX(true);
player.SetFlipY(true);
```

## Color tint

```cpp
player.SetColor({1.0f, 0.5f, 0.5f, 1.0f});
```

`{1, 1, 1, 1}` keeps the original texture colors.
