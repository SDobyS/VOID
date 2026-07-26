#include "Renderer.h"
#include "utils/Log.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <utility>

namespace voidx {
    constexpr uint32_t MAX_QUADS = 20000;
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

    void Renderer::BeginScene(const OrthographicCamera& camera) {
        s_DefaultShader->Use();
        s_DefaultShader->SetUniformMat4f("u_ViewProj", glm::value_ptr(camera.GetViewProjectionMatrix()));

        int samplers[32];
        for (int i = 0; i < 32; i++) samplers[i] = i;
        s_DefaultShader->SetUniform1iv("u_Textures", 32, samplers);

        s_IndexCount = 0;
        s_VertexBufferPtr = s_VertexBufferBase.data();
        s_TextureSlotIndex = 1;
        s_Stats.DrawCalls = 0;
        s_Stats.QuadCount = 0;
    }

    void Renderer::EndScene() {
        uint32_t dataSize = reinterpret_cast<uint8_t*>(s_VertexBufferPtr) - reinterpret_cast<uint8_t*>(s_VertexBufferBase.data());
        glBindBuffer(GL_ARRAY_BUFFER, s_QuadVBO);
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
        if (s_IndexCount >= MAX_INDICES) { EndScene(); s_IndexCount = 0; s_VertexBufferPtr = s_VertexBufferBase.data(); s_TextureSlotIndex = 1; }

        float texIndex = 0.0f;
        for (uint32_t i = 1; i < s_TextureSlotIndex; i++) {
            if (s_TextureSlots[i] == texture.GetID()) { texIndex = (float)i; break; }
        }
        if (texIndex == 0.0f) {
            if (s_TextureSlotIndex >= s_MaxTextureSlots) { EndScene(); s_IndexCount = 0; s_VertexBufferPtr = s_VertexBufferBase.data(); s_TextureSlotIndex = 1; }
            texIndex = (float)s_TextureSlotIndex; s_TextureSlots[s_TextureSlotIndex] = texture.GetID(); s_TextureSlotIndex++;
        }

        glm::vec2 offset(size.x * pivot.x, size.y * pivot.y);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position - offset, 0.0f));

        if (rotation != 0.0f) {
            model = glm::translate(model, glm::vec3(offset, 0.0f));
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::translate(model, glm::vec3(-offset, 0.0f));
        }
        model = glm::scale(model, glm::vec3(size, 1.0f));

        glm::vec2 v0 = glm::vec2(model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glm::vec2 v1 = glm::vec2(model * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        glm::vec2 v2 = glm::vec2(model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        glm::vec2 v3 = glm::vec2(model * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        float u0 = flipX ? 1.0f : 0.0f;
        float u1 = flipX ? 0.0f : 1.0f;
        float v_0 = flipY ? 1.0f : 0.0f;
        float v_1 = flipY ? 0.0f : 1.0f;

        s_VertexBufferPtr->Position = v0; s_VertexBufferPtr->Color = color; s_VertexBufferPtr->TexCoords = {u0, v_0}; s_VertexBufferPtr->TexIndex = texIndex; s_VertexBufferPtr++;
        s_VertexBufferPtr->Position = v1; s_VertexBufferPtr->Color = color; s_VertexBufferPtr->TexCoords = {u1, v_0}; s_VertexBufferPtr->TexIndex = texIndex; s_VertexBufferPtr++;
        s_VertexBufferPtr->Position = v2; s_VertexBufferPtr->Color = color; s_VertexBufferPtr->TexCoords = {u1, v_1}; s_VertexBufferPtr->TexIndex = texIndex; s_VertexBufferPtr++;
        s_VertexBufferPtr->Position = v3; s_VertexBufferPtr->Color = color; s_VertexBufferPtr->TexCoords = {u0, v_1}; s_VertexBufferPtr->TexIndex = texIndex; s_VertexBufferPtr++;

        s_IndexCount += 6;
        s_Stats.QuadCount++;
    }

    void Renderer::DrawQuadUV(const Texture& texture, const glm::vec2& position, const glm::vec2& size, const glm::vec2 uv[4], const glm::vec4& color, const glm::vec2& pivot, float rotation, bool flipX, bool flipY) {
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
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position - offset, 0.0f));

        if (rotation != 0.0f) {
            model = glm::translate(model, glm::vec3(offset, 0.0f));
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::translate(model, glm::vec3(-offset, 0.0f));
        }
        model = glm::scale(model, glm::vec3(size, 1.0f));

        glm::vec2 v0 = glm::vec2(model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glm::vec2 v1 = glm::vec2(model * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        glm::vec2 v2 = glm::vec2(model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        glm::vec2 v3 = glm::vec2(model * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

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
        if (s_IndexCount >= MAX_INDICES) {
            EndScene();
            s_IndexCount = 0;
            s_VertexBufferPtr = s_VertexBufferBase.data();
            s_TextureSlotIndex = 1;
        }

        float texIndex = 0.0f;

        glm::vec2 offset = size * 0.5f;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        if (rotation != 0.0f) {
            model = glm::translate(model, glm::vec3(offset, 0.0f));
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::translate(model, glm::vec3(-offset, 0.0f));
        }
        model = glm::scale(model, glm::vec3(size, 1.0f));

        glm::vec2 v0 = glm::vec2(model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glm::vec2 v1 = glm::vec2(model * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        glm::vec2 v2 = glm::vec2(model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        glm::vec2 v3 = glm::vec2(model * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        s_VertexBufferPtr->Position = v0;
        s_VertexBufferPtr->Color = color;
        s_VertexBufferPtr->TexCoords = {0.0f, 0.0f};
        s_VertexBufferPtr->TexIndex = texIndex;
        s_VertexBufferPtr++;
        s_VertexBufferPtr->Position = v1;
        s_VertexBufferPtr->Color = color;
        s_VertexBufferPtr->TexCoords = {1.0f, 0.0f};
        s_VertexBufferPtr->TexIndex = texIndex;
        s_VertexBufferPtr++;
        s_VertexBufferPtr->Position = v2;
        s_VertexBufferPtr->Color = color;
        s_VertexBufferPtr->TexCoords = {1.0f, 1.0f};
        s_VertexBufferPtr->TexIndex = texIndex;
        s_VertexBufferPtr++;
        s_VertexBufferPtr->Position = v3;
        s_VertexBufferPtr->Color = color;
        s_VertexBufferPtr->TexCoords = {0.0f, 1.0f};
        s_VertexBufferPtr->TexIndex = texIndex;
        s_VertexBufferPtr++;

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
        s_VertexBufferBase.clear();
        delete s_DefaultShader;
        glDeleteTextures(1, &s_WhiteTextureID);
        glDeleteVertexArrays(1, &s_QuadVAO);
        glDeleteBuffers(1, &s_QuadVBO);
        glDeleteBuffers(1, &s_QuadEBO);
    }
}