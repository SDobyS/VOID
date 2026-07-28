# Framebuffer

Framebuffers allow rendering into textures instead of directly to the screen.
Typical uses: render-to-texture, post-processing, off-screen rendering,
mini-maps, lighting effects.

## Creating

```cpp
Framebuffer framebuffer(1280, 720);
```

## Binding

```cpp
framebuffer.Bind();
// ... render into the framebuffer ...
framebuffer.Unbind(); // back to the default framebuffer
```

## Resizing

```cpp
framebuffer.Resize(1920, 1080);
```

## Reading the result

```cpp
GLuint texture = framebuffer.GetTextureID();
```

Like other GPU resources, `Framebuffer` is move-only.
