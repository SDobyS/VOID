#include "Sprite.h"
#include "Renderer.h"
#include "Camera/OrthographicCamera.h"

Sprite::Sprite(const std::string& texturePath, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    : m_Texture(), m_Position(position), m_Size(size), m_Color(color) {
    m_Texture.Load(texturePath);
}

void Sprite::Draw(const OrthographicCamera& camera) {
    Renderer::BeginScene(camera);
    Renderer::DrawQuad(m_Texture, m_Position, m_Size, m_Rotation, m_Color);
}