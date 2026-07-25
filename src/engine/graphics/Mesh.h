#pragma once

#include <glad/gl.h>
#include "VertexArray.h"

class Mesh {
public:
    Mesh() = default;
    ~Mesh();

    bool Create(
        const float* vertices,
        unsigned int verticesSize,
        const unsigned int* indices,
        unsigned int indicesSize
    );

    void Bind() const;
    void Draw() const;

private:
    VertexArray m_VAO;
    GLuint m_VBO = 0;
    GLuint m_EBO = 0;
    GLsizei m_IndexCount = 0;
};