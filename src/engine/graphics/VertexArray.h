#pragma once
#include <glad/gl.h>

namespace voidx {
    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        VertexArray(VertexArray&& other) noexcept;
        VertexArray& operator=(VertexArray&& other) noexcept;

        void Bind() const;
        void Unbind() const;
        GLuint GetID() const { return m_ID; }

    private:
        GLuint m_ID = 0;
    };
}