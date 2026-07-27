#include "VertexArray.h"
#include "utils/Log.h"
#include <utility>

namespace voidx {
    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_ID);
        Log::Debug("VertexArray", "Created (ID: " + std::to_string(m_ID) + ")");
    }

    VertexArray::~VertexArray() {
        if (m_ID) {
            Log::Debug("VertexArray", "Destroyed (ID: " + std::to_string(m_ID) + ")");
            glDeleteVertexArrays(1, &m_ID);
            m_ID = 0;
        }
    }

    VertexArray::VertexArray(VertexArray&& other) noexcept
    : m_ID(std::exchange(other.m_ID, 0)) {
        Log::Debug("VertexArray", "Move constructor called");
    }

    VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
        if (this != &other) {
            Log::Debug("VertexArray", "Move assignment called. Deleting old ID: " + std::to_string(m_ID));

            if (m_ID) glDeleteVertexArrays(1, &m_ID);

            m_ID = std::exchange(other.m_ID, 0);
        }
        return *this;
    }

    void VertexArray::Bind() const {
        Log::Debug("VertexArray", "Binding ID: " + std::to_string(m_ID));
        glBindVertexArray(m_ID);
    }

    void VertexArray::Unbind() const {
        Log::Debug("VertexArray", "Unbinding (binding 0)");
        glBindVertexArray(0);
    }
}