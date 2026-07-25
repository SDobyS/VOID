#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top) {
    RecalculateProjectionMatrix();
    RecalculateViewMatrix();
}

void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) {
    m_Left = left;
    m_Right = right;
    m_Bottom = bottom;
    m_Top = top;
    RecalculateProjectionMatrix();
}

void OrthographicCamera::SetPosition(const glm::vec3& position) {
    m_Position = position;
    RecalculateViewMatrix();
}

void OrthographicCamera::SetRotation(float rotation) {
    m_Rotation = rotation;
    RecalculateViewMatrix();
}

void OrthographicCamera::SetZoom(float zoom) {
    m_Zoom = std::max(zoom, 0.1f);
    RecalculateProjectionMatrix();
    m_ViewProjection = m_Projection * m_View;
}

void OrthographicCamera::RecalculateProjectionMatrix() {
    float scaledLeft   = m_Left   / m_Zoom;
    float scaledRight  = m_Right  / m_Zoom;
    float scaledBottom = m_Bottom / m_Zoom;
    float scaledTop    = m_Top    / m_Zoom;

    m_Projection = glm::ortho(scaledLeft, scaledRight, scaledBottom, scaledTop, -1.0f, 1.0f);
    m_ViewProjection = m_Projection * m_View;
}

void OrthographicCamera::RecalculateViewMatrix() {
    glm::mat4 transform = glm::translate(
        glm::mat4(1.0f),
        m_Position) * glm::rotate(glm::mat4(1.0f),
        glm::radians(m_Rotation),
        glm::vec3(0, 0, 1)
    );

    m_View = glm::inverse(transform);
    m_ViewProjection = m_Projection * m_View;
}