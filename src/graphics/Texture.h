#pragma once
#include <glad/gl.h>
#include <string>

class Texture {
public:
    Texture(const std::string& path);
    ~Texture();
    void Bind(GLuint unit = 0) const;
    void Unbind() const;
    [[nodiscard]] GLuint GetID() const { return m_RendererID; }
    [[nodiscard]] int GetWidth() const { return m_Width; }
    [[nodiscard]] int GetHeight() const { return m_Height; }

private:
    GLuint m_RendererID = 0;
    int m_Width = 0;
    int m_Height = 0;
};