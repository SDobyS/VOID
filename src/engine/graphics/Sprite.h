#pragma once
#include "Texture.h"
#include <glm/glm.hpp>

class OrthographicCamera;

class Sprite {
public:
    Sprite(const std::string& texturePath, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = {1,1,1,1});
    void Draw(const OrthographicCamera& camera);

    void SetPosition(const glm::vec2& pos) { m_Position = pos; }
    void SetRotation(float rotation) { m_Rotation = rotation; }

    glm::vec2 GetPosition() const { return m_Position; }
    float GetRotation() const { return m_Rotation; }

private:
    Texture m_Texture;
    glm::vec2 m_Position;
    glm::vec2 m_Size;
    glm::vec4 m_Color;
    float m_Rotation = 0.0f;
};