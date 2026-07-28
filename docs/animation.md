# Animation

`Animation` is responsible for sprite sheet playback. It stores the current
frame, frame size, animation speed, frame count and frames-per-row.

## Creating an animation

```cpp
Animation animation(
    texture,
    64,   // frameWidth
    64,   // frameHeight
    8,    // frameCount
    0.1f, // frameTime (seconds per frame)
    4     // framesPerRow
);
```

| Parameter | Description |
|-----------|-------------|
| texture | Sprite sheet |
| frameWidth | Width of one frame |
| frameHeight | Height of one frame |
| frameCount | Total frame count |
| frameTime | Seconds per frame |
| framesPerRow | Frames contained in one row |

## Updating

```cpp
animation.Update(dt);
```

## Changing playback speed

```cpp
animation.SetFrameTime(0.05f);
```

UV coordinates for the current frame are generated automatically.
