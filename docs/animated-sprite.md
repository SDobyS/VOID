# AnimatedSprite

`AnimatedSprite` combines `Sprite` and `Animation` into a single high-level
object. Instead of calculating UV coordinates manually, the current animation
frame is rendered automatically.

## Creating

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

## Transform & appearance

```cpp
player.SetPosition({200, 300});
player.SetRotation(20.0f);
player.SetFlipX(true);
player.SetColor({1, 1, 1, 1});
```

## Updating and drawing

```cpp
player.Update(dt);
player.Draw();
```
