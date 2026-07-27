#include "Renderer.h"
#include "utils/Log.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <utility>

namespace voidx {
    constexpr uint32_t MAX_QUADS = 100000;
    constexpr uint32_t MAX_VERTICES = MAX_QUADS * 4;
    constexpr uint32_t MAX_INDICES = MAX_QUADS * 6;

    Shader* Renderer::s_DefaultShader = nullptr;
    GLuint Renderer::s_QuadVAO = 0;
    GLuint Renderer::s_QuadVBO = 0;
    GLuint Renderer::s_QuadEBO = 0;
    RendererStats Renderer::s_Stats;

    uint32_t Renderer::s_IndexCount = 0;
    std::vector<BatchVertex> Renderer::s_VertexBufferBase;
    BatchVertex* Renderer::s_VertexBufferPtr = nullptr;

    uint32_t Renderer::s_MaxTextureSlots = 32;
    std::vector<GLuint> Renderer::s_TextureSlots;
    uint32_t Renderer::s_TextureSlotIndex = 1;

    GLuint Renderer::s_WhiteTextureID = 0;

    bool Renderer::Init() {
        s_DefaultShader = new Shader();
        if (!s_DefaultShader->Load("../assets/engine/shaders/default.vert", "../assets/engine/shaders/default.frag")) {
            Log::Error("Renderer", "Failed to load default shader! Rendering will not work.");
            return false;
        }

        s_VertexBufferBase.resize(MAX_VERTICES);

        glGenVertexArrays(1, &s_QuadVAO);
        glGenBuffers(1, &s_QuadVBO);
        glGenBuffers(1, &s_QuadEBO);

        glBindVertexArray(s_QuadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, s_QuadVBO);
        glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(BatchVertex), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), reinterpret_cast<void*>(offsetof(BatchVertex, Position)));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), reinterpret_cast<void*>(offsetof(BatchVertex, Color)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), reinterpret_cast<void*>(offsetof(BatchVertex, TexCoords)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), reinterpret_cast<void*>(offsetof(BatchVertex, TexIndex)));

        std::vector<uint32_t> indices(MAX_INDICES);
        uint32_t offset = 0;
        for (uint32_t i = 0; i < MAX_INDICES; i += 6) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            offset += 4;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_QuadEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);
        glBindVertexArray(0);

        int maxTexUnits = 32;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTexUnits);
        s_MaxTextureSlots = std::min(32, maxTexUnits);
        Log::Info("Renderer", "Max texture units supported: " + std::to_string(s_MaxTextureSlots));
        s_TextureSlots.resize(s_MaxTextureSlots);

        glGenTextures(1, &s_WhiteTextureID);
        glBindTexture(GL_TEXTURE_2D, s_WhiteTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        uint32_t whiteTextureData = 0xffffffff;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &whiteTextureData);

        s_TextureSlots[0] = s_WhiteTextureID;

        Log::Success("Batch Renderer initialized");
        return true;
    }

    glm::vec4 Renderer::s_CameraBounds = {0.0f, 0.0f, 0.0f, 0.0f};

    void Renderer::BeginScene(const OrthographicCamera& camera) {
        s_DefaultShader->Use();
        s_DefaultShader->SetUniformMat4f("u_ViewProj", glm::value_ptr(camera.GetViewProjectionMatrix()));

        int samplers[32];
        for (int i = 0; i < 32; i++) samplers[i] = i;
        s_DefaultShader->SetUniform1iv("u_Textures", 32, samplers);

        s_CameraBounds = camera.GetVisibleBounds();
        s_IndexCount = 0;
        s_VertexBufferPtr = s_VertexBufferBase.data();
        s_TextureSlotIndex = 1;
        s_Stats.DrawCalls = 0;
        s_Stats.QuadCount = 0;
    }

    void Renderer::EndScene() {
        uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(s_VertexBufferPtr) - reinterpret_cast<uint8_t*>(s_VertexBufferBase.data()));

        glBindBuffer(GL_ARRAY_BUFFER, s_QuadVBO);
        glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(BatchVertex), nullptr, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, s_VertexBufferBase.data());

        Flush();
        s_Stats.DrawCalls++;
    }

    void Renderer::Flush() {
        for (uint32_t i = 0; i < s_TextureSlotIndex; i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, s_TextureSlots[i]);
        }

        glBindVertexArray(s_QuadVAO);
        glDrawElements(GL_TRIANGLES, s_IndexCount, GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::DrawQuad(const Texture& texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, const glm::vec2& pivot, bool flipX, bool flipY) {
        if (position.x + size.x < s_CameraBounds.x || position.x > s_CameraBounds.z ||
        position.y + size.y < s_CameraBounds.y || position.y > s_CameraBounds.w) {
                return;
        }
        if (s_IndexCount >= MAX_INDICES) {
            EndScene();
            s_IndexCount = 0;
            s_VertexBufferPtr = s_VertexBufferBase.data();
            s_TextureSlotIndex = 1;
        }

        float texIndex = 0.0f;
        for (uint32_t i = 1; i < s_TextureSlotIndex; i++) {
            if (s_TextureSlots[i] == texture.GetID()) {
                texIndex = static_cast<float>(i);
                break;
            }
        }
        if (texIndex == 0.0f) {
            if (s_TextureSlotIndex >= s_MaxTextureSlots) {
                EndScene();
                s_IndexCount = 0;
                s_VertexBufferPtr = s_VertexBufferBase.data();
                s_TextureSlotIndex = 1;
            }
            texIndex = static_cast<float>(s_TextureSlotIndex);
            s_TextureSlots[s_TextureSlotIndex] = texture.GetID();
            s_TextureSlotIndex++;
        }

        glm::vec2 offset(size.x * pivot.x, size.y * pivot.y);
        glm::vec2 localPos = position - offset;

        glm::vec2 v0, v1, v2, v3;

        if (rotation == 0.0f) {
            v0 = localPos;
            v1 = {localPos.x + size.x, localPos.y};
            v2 = {localPos.x + size.x, localPos.y + size.y};
            v3 = {localPos.x, localPos.y + size.y};
        } else {
            float rad = glm::radians(rotation);
            float cosA = std::cos(rad);
            float sinA = std::sin(rad);

            glm::vec2 center = position;

            glm::vec2 l0 = {-offset.x, -offset.y};
            glm::vec2 l1 = {size.x - offset.x, -offset.y};
            glm::vec2 l2 = {size.x - offset.x, size.y - offset.y};
            glm::vec2 l3 = {-offset.x, size.y - offset.y};

            v0 = center + glm::vec2(l0.x * cosA - l0.y * sinA, l0.x * sinA + l0.y * cosA);
            v1 = center + glm::vec2(l1.x * cosA - l1.y * sinA, l1.x * sinA + l1.y * cosA);
            v2 = center + glm::vec2(l2.x * cosA - l2.y * sinA, l2.x * sinA + l2.y * cosA);
            v3 = center + glm::vec2(l3.x * cosA - l3.y * sinA, l3.x * sinA + l3.y * cosA);
        }

        float u0 = flipX ? 1.0f : 0.0f;
        float u1 = flipX ? 0.0f : 1.0f;
        float v0_tex = flipY ? 1.0f : 0.0f;
        float v1_tex = flipY ? 0.0f : 1.0f;

        s_VertexBufferPtr->Position = v0; s_VertexBufferPtr->Color = color; s_VertexBufferPtr->TexCoords = {u0, v0_tex}; s_VertexBufferPtr->TexIndex = texIndex; s_VertexBufferPtr++;
        s_VertexBufferPtr->Position = v1; s_VertexBufferPtr->Color = color; s_VertexBufferPtr->TexCoords = {u1, v0_tex}; s_VertexBufferPtr->TexIndex = texIndex; s_VertexBufferPtr++;
        s_VertexBufferPtr->Position = v2; s_VertexBufferPtr->Color = color; s_VertexBufferPtr->TexCoords = {u1, v1_tex}; s_VertexBufferPtr->TexIndex = texIndex; s_VertexBufferPtr++;
        s_VertexBufferPtr->Position = v3; s_VertexBufferPtr->Color = color; s_VertexBufferPtr->TexCoords = {u0, v1_tex}; s_VertexBufferPtr->TexIndex = texIndex; s_VertexBufferPtr++;

        s_IndexCount += 6;
        s_Stats.QuadCount++;
    }

    void Renderer::DrawQuadUV(const Texture& texture, const glm::vec2& position, const glm::vec2& size, const glm::vec2 uv[4], const glm::vec4& color, const glm::vec2& pivot, float rotation, bool flipX, bool flipY) {
        if (position.x + size.x < s_CameraBounds.x || position.x > s_CameraBounds.z ||
        position.y + size.y < s_CameraBounds.y || position.y > s_CameraBounds.w) {
                return;
        }
        if (s_IndexCount >= MAX_INDICES) {
            EndScene();
            s_IndexCount = 0;
            s_VertexBufferPtr = s_VertexBufferBase.data();
            s_TextureSlotIndex = 1;
        }

        float texIndex = 0.0f;
        for (uint32_t i = 1; i < s_TextureSlotIndex; i++) {
            if (s_TextureSlots[i] == texture.GetID()) {
                texIndex = static_cast<float>(i);
                break;
            }
        }
        if (texIndex == 0.0f) {
            if (s_TextureSlotIndex >= s_MaxTextureSlots) {
                EndScene();
                s_IndexCount = 0;
                s_VertexBufferPtr = s_VertexBufferBase.data();
                s_TextureSlotIndex = 1;
            }
            texIndex = static_cast<float>(s_TextureSlotIndex);
            s_TextureSlots[s_TextureSlotIndex] = texture.GetID();
            s_TextureSlotIndex++;
        }

        glm::vec2 offset(size.x * pivot.x, size.y * pivot.y);
        glm::vec2 localPos = position - offset;

        glm::vec2 v0, v1, v2, v3;

        if (rotation == 0.0f) {
            v0 = localPos;
            v1 = {localPos.x + size.x, localPos.y};
            v2 = {localPos.x + size.x, localPos.y + size.y};
            v3 = {localPos.x, localPos.y + size.y};
        } else {
            float rad = glm::radians(rotation);
            float cosA = std::cos(rad);
            float sinA = std::sin(rad);

            glm::vec2 center = position;

            glm::vec2 l0 = {-offset.x, -offset.y};
            glm::vec2 l1 = {size.x - offset.x, -offset.y};
            glm::vec2 l2 = {size.x - offset.x, size.y - offset.y};
            glm::vec2 l3 = {-offset.x, size.y - offset.y};

            v0 = center + glm::vec2(l0.x * cosA - l0.y * sinA, l0.x * sinA + l0.y * cosA);
            v1 = center + glm::vec2(l1.x * cosA - l1.y * sinA, l1.x * sinA + l1.y * cosA);
            v2 = center + glm::vec2(l2.x * cosA - l2.y * sinA, l2.x * sinA + l2.y * cosA);
            v3 = center + glm::vec2(l3.x * cosA - l3.y * sinA, l3.x * sinA + l3.y * cosA);
        }

        glm::vec2 t0 = uv[0], t1 = uv[1], t2 = uv[2], t3 = uv[3];
        if (flipX) { std::swap(t0, t1); std::swap(t3, t2); }
        if (flipY) { std::swap(t0, t3); std::swap(t1, t2); }

        s_VertexBufferPtr->Position = v0;
        s_VertexBufferPtr->Color = color;
        s_VertexBufferPtr->TexCoords = t0;
        s_VertexBufferPtr->TexIndex = texIndex;
        s_VertexBufferPtr++;

        s_VertexBufferPtr->Position = v1;
        s_VertexBufferPtr->Color = color;
        s_VertexBufferPtr->TexCoords = t1;
        s_VertexBufferPtr->TexIndex = texIndex;
        s_VertexBufferPtr++;

        s_VertexBufferPtr->Position = v2;
        s_VertexBufferPtr->Color = color;
        s_VertexBufferPtr->TexCoords = t2;
        s_VertexBufferPtr->TexIndex = texIndex;
        s_VertexBufferPtr++;

        s_VertexBufferPtr->Position = v3;
        s_VertexBufferPtr->Color = color;
        s_VertexBufferPtr->TexCoords = t3;
        s_VertexBufferPtr->TexIndex = texIndex;
        s_VertexBufferPtr++;

        s_IndexCount += 6;
        s_Stats.QuadCount++;
    }

    void Renderer::DrawColorQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation) {
        if (position.x + size.x < s_CameraBounds.x || position.x > s_CameraBounds.z ||
        position.y + size.y < s_CameraBounds.y || position.y > s_CameraBounds.w) {
                return;
        }
        if (s_IndexCount >= MAX_INDICES) {
            EndScene();
            s_IndexCount = 0;
            s_VertexBufferPtr = s_VertexBufferBase.data();
            s_TextureSlotIndex = 1;
        }

        float texIndex = 0.0f;
        glm::vec2 v0, v1, v2, v3;

        if (rotation == 0.0f) {
            v0 = position;
            v1 = {position.x + size.x, position.y};
            v2 = {position.x + size.x, position.y + size.y};
            v3 = {position.x, position.y + size.y};
        } else {
            float rad = glm::radians(rotation);
            float cosA = std::cos(rad);
            float sinA = std::sin(rad);

            glm::vec2 offset = size * 0.5f;
            glm::vec2 center = position + offset;

            glm::vec2 local0 = -offset;
            glm::vec2 local1 = {size.x - offset.x, -offset.y};
            glm::vec2 local2 = {size.x - offset.x, size.y - offset.y};
            glm::vec2 local3 = {-offset.x, size.y - offset.y};

            v0 = center + glm::vec2(local0.x * cosA - local0.y * sinA, local0.x * sinA + local0.y * cosA);
            v1 = center + glm::vec2(local1.x * cosA - local1.y * sinA, local1.x * sinA + local1.y * cosA);
            v2 = center + glm::vec2(local2.x * cosA - local2.y * sinA, local2.x * sinA + local2.y * cosA);
            v3 = center + glm::vec2(local3.x * cosA - local3.y * sinA, local3.x * sinA + local3.y * cosA);
        }

        s_VertexBufferPtr->Position = v0; s_VertexBufferPtr->Color = color; s_VertexBufferPtr->TexCoords = {0.0f, 0.0f}; s_VertexBufferPtr->TexIndex = texIndex; s_VertexBufferPtr++;
        s_VertexBufferPtr->Position = v1; s_VertexBufferPtr->Color = color; s_VertexBufferPtr->TexCoords = {1.0f, 0.0f}; s_VertexBufferPtr->TexIndex = texIndex; s_VertexBufferPtr++;
        s_VertexBufferPtr->Position = v2; s_VertexBufferPtr->Color = color; s_VertexBufferPtr->TexCoords = {1.0f, 1.0f}; s_VertexBufferPtr->TexIndex = texIndex; s_VertexBufferPtr++;
        s_VertexBufferPtr->Position = v3; s_VertexBufferPtr->Color = color; s_VertexBufferPtr->TexCoords = {0.0f, 1.0f}; s_VertexBufferPtr->TexIndex = texIndex; s_VertexBufferPtr++;

        s_IndexCount += 6;
        s_Stats.QuadCount++;
    }

    void Renderer::DrawText(const Font& font, const std::string& text, const glm::vec2& position, float scale, const glm::vec4& color) {
        float x = position.x;
        float y = position.y;
        const stbtt_bakedchar* glyphs = font.GetGlyphs();
        const Texture& tex = font.GetTexture();

        for (char c : text) {
            if (c < 32 || c > 126) c = ' ';
            const stbtt_bakedchar& g = glyphs[c - 32];

            glm::vec2 quadPos = { x + g.xoff * scale, y + g.yoff * scale };
            glm::vec2 quadSize = { (g.x1 - g.x0) * scale, (g.y1 - g.y0) * scale };

            float texW = tex.GetWidth();
            float texH = tex.GetHeight();

            glm::vec2 uv[4] = {
                { g.x0 / texW, g.y0 / texH },
                { g.x1 / texW, g.y0 / texH },
                { g.x1 / texW, g.y1 / texH },
                { g.x0 / texW, g.y1 / texH }
            };

            DrawQuadUV(tex, quadPos, quadSize, uv, color);
            x += g.xadvance * scale;
        }
    }

    void Renderer::Shutdown() {
        Log::Info("Renderer", "Shutting down and cleaning resources...");

        Log::Debug("Renderer", "Clearing vertex buffer base...");
        s_VertexBufferBase.clear();

        if (s_DefaultShader) {
            Log::Debug("Renderer", "Deleting default shader...");
            delete s_DefaultShader;
            s_DefaultShader = nullptr;
        }

        if (s_WhiteTextureID) {
            Log::Debug("Renderer", "Deleting white fallback texture (ID: " + std::to_string(s_WhiteTextureID) + ")...");
            glDeleteTextures(1, &s_WhiteTextureID);
            s_WhiteTextureID = 0;
        }
        if (s_QuadVAO) {
            Log::Debug("Renderer", "Deleting Quad VAO (ID: " + std::to_string(s_QuadVAO) + ")...");
            glDeleteVertexArrays(1, &s_QuadVAO);
            s_QuadVAO = 0;
        }
        if (s_QuadVBO) {
            Log::Debug("Renderer", "Deleting Quad VBO (ID: " + std::to_string(s_QuadVBO) + ")...");
            glDeleteBuffers(1, &s_QuadVBO);
            s_QuadVBO = 0;
        }
        if (s_QuadEBO) {
            Log::Debug("Renderer", "Deleting Quad EBO (ID: " + std::to_string(s_QuadEBO) + ")...");
            glDeleteBuffers(1, &s_QuadEBO);
            s_QuadEBO = 0;
        }

        Log::Debug("Renderer", "Clearing texture slots and batch state...");
        s_TextureSlots.clear();
        s_TextureSlotIndex = 1;
        s_IndexCount = 0;
        s_VertexBufferPtr = nullptr;
        s_Stats = RendererStats();

        Log::Success("Renderer", "Shutdown complete.");
    }
}