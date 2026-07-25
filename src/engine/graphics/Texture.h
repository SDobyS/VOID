#pragma once
#include <glad/gl.h>
#include <string>

class Texture {
public:
    Texture() = default;
    ~Texture();

    bool Load(const std::string& path);
    void Bind() const;
    void Unbind() const;

    GLuint GetID() const { return m_ID; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

private:
    GLuint m_ID = 0;
    int m_Width = 0;
    int m_Height = 0;
};