#include "Mesh.h"
#include "utils/Log.h"

bool Mesh::Create(
    const float* vertices,
    unsigned int verticesSize,
    const unsigned int* indices,
    unsigned int indicesSize
) {
    m_IndexCount = indicesSize / sizeof(unsigned int);

    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    m_VAO.Bind();

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    // position (vec2)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // color (vec3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texcoord (vec2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), reinterpret_cast<void*>(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_VAO.Unbind();

    Log::Success("Mesh created successfully", "Index count: " + std::to_string(m_IndexCount));
    return true;
}

void Mesh::Bind() const {
    m_VAO.Bind();
}

void Mesh::Draw() const {
    m_VAO.Bind();
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
}

Mesh::~Mesh() {
    if (m_VBO) {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
    if (m_EBO) {
        glDeleteBuffers(1, &m_EBO);
        m_EBO = 0;
    }
    Log::Debug("Mesh resources destroyed");
}