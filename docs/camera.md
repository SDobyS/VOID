# Camera

The renderer uses an orthographic camera, and automatically uses its
ViewProjection matrix when a scene begins.

## Creating a camera

```cpp
OrthographicCamera camera(
    0.0f,
    1280.0f,
    720.0f,
    0.0f
);
```

## Moving the camera

```cpp
camera.SetPosition({200.0f, 100.0f, 0.0f});
```

## Rotating the camera

```cpp
camera.SetRotation(45.0f);
```

## Zooming

```cpp
camera.SetZoom(2.0f);
```

## Using it in a scene

```cpp
Renderer::BeginScene(camera);
```
