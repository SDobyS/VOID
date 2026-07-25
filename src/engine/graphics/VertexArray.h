#pragma once

#include <glad/gl.h>

class VertexArray {
public:
    VertexArray();
    ~VertexArray();
    void Bind() const;
    void Unbind() const;
    GLuint GetID() const;

private:
    GLuint m_ID = 0;
};