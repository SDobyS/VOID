#include "Mesh.h"
#include "utils/Log.h"
#include <utility>

namespace voidx {
    bool Mesh::Create(const float* vertices, unsigned int verticesSize, const unsigned int* indices, unsigned int indicesSize) {
        m_IndexCount = indicesSize / sizeof(unsigned int);

        Log::Info("Mesh", "Creating mesh (Vertices: " + std::to_string(verticesSize / (7 * sizeof(float))) + ", Indices: " + std::to_string(m_IndexCount) + ")...");

        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
        m_VAO.Bind();

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), reinterpret_cast<void*>(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        m_VAO.Unbind();

        Log::Success("Mesh", "Created successfully");
        return true;
    }

    void Mesh::Bind() const {
        Log::Debug("Mesh", "Binding Mesh");
        m_VAO.Bind();
    }

    void Mesh::Draw() const {
        Log::Debug("Mesh", "Drawing Mesh (Indices: " + std::to_string(m_IndexCount) + ")");
        m_VAO.Bind();
        glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
    }

    Mesh::Mesh(Mesh&& other) noexcept
    : m_VAO(std::move(other.m_VAO)),
      m_VBO(std::exchange(other.m_VBO, 0)),
      m_EBO(std::exchange(other.m_EBO, 0)),
      m_IndexCount(other.m_IndexCount) {
        Log::Debug("Mesh", "Move constructor called");
    }

    Mesh& Mesh::operator=(Mesh&& other) noexcept {
        if (this != &other) {
            Log::Debug("Mesh", "Move assignment called. Cleaning up old VBO/EBO...");

            if (m_VBO) glDeleteBuffers(1, &m_VBO);
            if (m_EBO) glDeleteBuffers(1, &m_EBO);

            m_VAO = std::move(other.m_VAO);
            m_VBO = std::exchange(other.m_VBO, 0);
            m_EBO = std::exchange(other.m_EBO, 0);
            m_IndexCount = other.m_IndexCount;
        }
        return *this;
    }

    Mesh::~Mesh() {
        if (m_VBO || m_EBO) {
            Log::Debug("Mesh", "Resources destroyed (VBO: " + std::to_string(m_VBO) + ", EBO: " + std::to_string(m_EBO) + ")");
            if (m_VBO) { glDeleteBuffers(1, &m_VBO); m_VBO = 0; }
            if (m_EBO) { glDeleteBuffers(1, &m_EBO); m_EBO = 0; }
        }
    }
}