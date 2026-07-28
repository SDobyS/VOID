# Texture

Textures are GPU resources managed automatically via RAII. `Texture` supports
move semantics but **cannot be copied**.

## Loading

```cpp
Texture texture;
texture.Load("assets/player.png");
```

## Binding

```cpp
texture.Bind();
```

To a specific slot:

```cpp
texture.Bind(3);
```

Unbind:

```cpp
texture.Unbind();
```

## Size and handle

```cpp
int width  = texture.GetWidth();
int height = texture.GetHeight();
GLuint id  = texture.GetID();
```

If a texture fails to load, the engine falls back to a placeholder texture rather
than crashing.
