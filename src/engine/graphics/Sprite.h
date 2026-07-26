#pragma once
#include "Texture.h"
#include <glm/glm.hpp>
#include <memory>

namespace voidx {
    class Sprite {
    public:
        Sprite() = default;
        Sprite(std::shared_ptr<Texture> texture, const glm::vec2& position = {0.0f, 0.0f}, const glm::vec2& size = {100.0f, 100.0f}, const glm::vec4& color = {1,1,1,1});

        void Draw();

        void SetPosition(const glm::vec2& pos) { m_Position = pos; }
        void SetRotation(float rotation) { m_Rotation = rotation; }
        void SetPivot(const glm::vec2& pivot) { m_Pivot = pivot; }
        void SetFlipX(bool flip) { m_FlipX = flip; }
        void SetFlipY(bool flip) { m_FlipY = flip; }
        void SetColor(const glm::vec4& color) { m_Color = color; }

        glm::vec2 GetPosition() const { return m_Position; }
        float GetRotation() const { return m_Rotation; }
        glm::vec2 GetPivot() const { return m_Pivot; }
        bool GetFlipX() const { return m_FlipX; }
        bool GetFlipY() const { return m_FlipY; }
        glm::vec4 GetColor() const { return m_Color; }

    private:
        std::shared_ptr<Texture> m_Texture;
        glm::vec2 m_Position;
        glm::vec2 m_Size;
        glm::vec4 m_Color;
        float m_Rotation = 0.0f;
        glm::vec2 m_Pivot = {0.5f, 0.5f};
        bool m_FlipX = false;
        bool m_FlipY = false;
    };
}