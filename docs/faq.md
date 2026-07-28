# FAQ

**Do I need vcpkg or another package manager?**
No. All dependencies (SDL3, GLAD, GLM, Dear ImGui, stb_image, stb_truetype,
nlohmann/json) are fetched automatically via CMake FetchContent.

**Can I copy a Texture / Shader / Font / Mesh / VertexArray / Framebuffer?**
No — these own OpenGL resources and are move-only by design, to avoid accidental
duplication of GPU handles. Use `std::move`.

**What happens if a texture fails to load?**
The engine falls back to a placeholder texture instead of crashing.

**Does VOID support Tiled's infinite maps or external TSX tilesets?**
Not currently. Supported: embedded tilesets, CSV tile layers, multiple visible
layers. Unsupported: external TSX tilesets, Base64-encoded layers, infinite maps.

**How many sprites can I draw per batch?**
Up to 100,000 quads / 400,000 vertices / 600,000 indices, and up to 32 texture
slots per batch (hardware dependent), before the renderer automatically flushes.

**How do I open the debug UI?**
Enable it via `config.showDebugUI = true` on `WindowConfig`, then toggle it at
runtime with **F8**. **F11** toggles fullscreen.

**What license is VOID under?**
MIT.