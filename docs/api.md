# API Reference

Quick reference for the lower-level building blocks not covered by their own
guide page, plus a summary table of the whole public API.

## Mesh

Stores custom geometry and owns its GPU buffers (auto-released on destruction).

```cpp
Mesh mesh;
mesh.Create(vertices, sizeof(vertices), indices, sizeof(indices));

mesh.Bind();
mesh.Draw();
```

## Shader

Wraps an OpenGL shader program.

```cpp
Shader shader;
shader.Load("default.vert", "default.frag");
shader.Use();
```

Uniforms:

```cpp
shader.SetUniform1i("u_Texture", 0);
shader.SetUniform1iv("u_Textures", 32, samplers);
shader.SetUniformMat4f("u_ViewProj", matrix);
shader.SetUniform4f("u_Color", {1, 1, 1, 1});
```

## Summary table

| Class | Header concept | Copy | Move |
|-------|-----------------|:----:|:----:|
| Texture | GPU texture | ❌ | ✅ |
| Shader | GL shader program | ❌ | ✅ |
| Font | Bitmap font atlas | ❌ | ✅ |
| Mesh | Custom geometry | ❌ | ✅ |
| VertexArray | GL vertex array object | ❌ | ✅ |
| Framebuffer | Render target | ❌ | ✅ |
| Sprite | Texture + transform | ✅ | ✅ |
| Animation | Sprite-sheet playback state | ✅ | ✅ |
| AnimatedSprite | Sprite + Animation | ✅ | ✅ |
| Tilemap | Tiled JSON map | — | — |
| ParticleSystem | Pooled particle emitter | — | — |
| OrthographicCamera | 2D camera | ✅ | ✅ |

See also: [Renderer](renderer.md), [Asset Manager](assets.md),
[Texture](texture.md), [Sprite](sprite.md), [Tilemap](tilemap.md),
[Particles](particles.md), [Framebuffer](framebuffer.md).
