#pragma once
#include <glm/glm.hpp>
#include <glad/gl.h>

namespace voidx {
    struct QuadRenderCommand {
        GLuint TextureID = 0;
        glm::vec2 Position = {0.0f, 0.0f};
        glm::vec2 Size = {1.0f, 1.0f};
        float Rotation = 0.0f;
        glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};
        glm::vec2 UV[4] = {{0,0}, {1,0}, {1,1}, {0,1}};
        glm::vec2 Pivot = {0.5f, 0.5f};
        bool FlipX = false;
        bool FlipY = false;
        bool UseUVs = false;
        float ZOrder = 0.0f;
        bool IsTransparent = false;
    };
}