#include "Sprite.h"
#include "Renderer.h"

namespace voidx {
    Sprite::Sprite(std::shared_ptr<Texture> texture, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
        : m_Texture(texture), m_Position(position), m_Size(size), m_Color(color) {}

    void Sprite::Draw() {
        if (!m_Texture) return;
        Renderer::DrawQuad(*m_Texture, m_Position, m_Size, m_Rotation, m_Color, m_Pivot, m_FlipX, m_FlipY);
    }
}