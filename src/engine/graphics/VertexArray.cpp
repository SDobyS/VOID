#include "VertexArray.h"
#include "utils/Log.h"

namespace voidx {
    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_ID);
        Log::Debug("VertexArray created", std::to_string(m_ID));
    }

    VertexArray::~VertexArray() {
        if (m_ID) {
            glDeleteVertexArrays(1, &m_ID);
            Log::Debug("VertexArray destroyed", std::to_string(m_ID));
            m_ID = 0;
        }
    }

    void VertexArray::Bind() const { glBindVertexArray(m_ID); }
    void VertexArray::Unbind() const { glBindVertexArray(0); }
}