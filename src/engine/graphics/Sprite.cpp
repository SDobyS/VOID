#include "Sprite.h"
#include "SceneRenderer.h"

namespace voidx {
    Sprite::Sprite(std::shared_ptr<Texture> texture, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
        : m_Texture(texture), m_Position(position), m_Size(size), m_Color(color) {}

    void Sprite::Draw() {
        if (!m_Texture) return;
        QuadRenderCommand cmd;
        cmd.TextureID = m_Texture->GetID();
        cmd.Position = m_Position;
        cmd.Size = m_Size;
        cmd.Rotation = m_Rotation;
        cmd.Color = m_Color;
        cmd.Pivot = m_Pivot;
        cmd.FlipX = m_FlipX;
        cmd.FlipY = m_FlipY;
        SceneRenderer::Submit(cmd);
    }
}