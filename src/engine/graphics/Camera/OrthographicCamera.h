#pragma once

#include <glm/glm.hpp>

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