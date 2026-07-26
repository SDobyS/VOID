#pragma once
#include <glad/gl.h>
#include "Shader.h"
#include "Texture.h"
#include "Camera/OrthographicCamera.h"
#include "Font.h"
#include <glm/glm.hpp>
#include <array>
#include <vector>

namespace voidx {
    struct BatchVertex {
        glm::vec2 Position;
        glm::vec4 Color;
        glm::vec2 TexCoords;
        float TexIndex;
    };

    struct RendererStats {
        uint32_t DrawCalls = 0;
        uint32_t QuadCount = 0;
    };

    class Renderer {
    public:
        static bool Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        static void DrawQuad(const Texture& texture, const glm::vec2& position, const glm::vec2& size, float rotation = 0.0f, const glm::vec4& color = {1,1,1,1}, const glm::vec2& pivot = {0.5f, 0.5f}, bool flipX = false, bool flipY = false);
        static void DrawQuadUV(const Texture& texture, const glm::vec2& position, const glm::vec2& size, const glm::vec2 uv[4], const glm::vec4& color, const glm::vec2& pivot = {0.0f, 0.0f}, float rotation = 0.0f, bool flipX = false, bool flipY = false);

        static void DrawColorQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation = 0.0f);
        static void DrawText(const Font& font, const std::string& text, const glm::vec2& position, float scale = 1.0f, const glm::vec4& color = {1,1,1,1});

        static RendererStats GetStats() { return s_Stats; }

    private:
        static void Flush();

        static Shader* s_DefaultShader;
        static GLuint s_QuadVAO;
        static GLuint s_QuadVBO;
        static GLuint s_QuadEBO;

        static uint32_t s_IndexCount;
        static std::vector<BatchVertex> s_VertexBufferBase;
        static BatchVertex* s_VertexBufferPtr;

        static uint32_t s_MaxTextureSlots;
        static std::vector<GLuint> s_TextureSlots;
        static uint32_t s_TextureSlotIndex;

        static GLuint s_WhiteTextureID;
        static RendererStats s_Stats;
    };
}