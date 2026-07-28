#pragma once
#include <glm/glm.hpp>
#include <algorithm>
#include <limits>

namespace voidx {
    class OrthographicCamera {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        void SetProjection(float left, float right, float bottom, float top);

        void SetPosition(const glm::vec3& position);
        [[nodiscard]] const glm::vec3& GetPosition() const { return m_Position; }

        void SetRotation(float rotation);
        [[nodiscard]] float GetRotation() const { return m_Rotation; }

        void SetZoom(float zoom);
        [[nodiscard]] float GetZoom() const { return m_Zoom; }

        [[nodiscard]] const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
        [[nodiscard]] const glm::mat4& GetViewMatrix() const { return m_View; }
        [[nodiscard]] const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjection; }

        [[nodiscard]] glm::vec4 GetVisibleBounds() const {
            glm::vec2 center(m_Position.x, m_Position.y);
            float rad = glm::radians(m_Rotation);
            float cosA = std::cos(rad);
            float sinA = std::sin(rad);

            glm::vec2 corners[4] = {
                {m_Left / m_Zoom, m_Bottom / m_Zoom},
                {m_Right / m_Zoom, m_Bottom / m_Zoom},
                {m_Right / m_Zoom, m_Top / m_Zoom},
                {m_Left / m_Zoom, m_Top / m_Zoom}
            };

            float minX = std::numeric_limits<float>::max();
            float maxX = std::numeric_limits<float>::lowest();
            float minY = std::numeric_limits<float>::max();
            float maxY = std::numeric_limits<float>::lowest();

            for (const auto& c : corners) {
                glm::vec2 rotated(
                    center.x + c.x * cosA - c.y * sinA,
                    center.y + c.x * sinA + c.y * cosA
                );
                minX = std::min(minX, rotated.x);
                maxX = std::max(maxX, rotated.x);
                minY = std::min(minY, rotated.y);
                maxY = std::max(maxY, rotated.y);
            }
            return { minX, minY, maxX, maxY };
        }

    private:
        void RecalculateViewMatrix();
        void RecalculateProjectionMatrix();

        glm::mat4 m_Projection;
        glm::mat4 m_View;
        glm::mat4 m_ViewProjection;

        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        float m_Rotation = 0.0f;
        float m_Zoom = 1.0f;
        float m_Left = 0.0f, m_Right = 0.0f, m_Bottom = 0.0f, m_Top = 0.0f;
    };
}