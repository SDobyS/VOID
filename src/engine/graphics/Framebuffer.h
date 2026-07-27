#pragma once
#include <glad/gl.h>

namespace voidx {
    class Framebuffer {
    public:
        Framebuffer(int width, int height);
        ~Framebuffer();

        Framebuffer(const Framebuffer&) = delete;
        Framebuffer& operator=(const Framebuffer&) = delete;

        Framebuffer(Framebuffer&& other) noexcept;
        Framebuffer& operator=(Framebuffer&& other) noexcept;

        void Bind() const;
        void Unbind() const;
        void Resize(int width, int height);
        GLuint GetTextureID() const { return m_TextureID; }
        [[nodiscard]] bool IsValid() const { return m_FBO != 0 && m_IsComplete; }

    private:
        GLuint m_FBO = 0;
        GLuint m_TextureID = 0;
        GLuint m_RBO = 0;
        int m_Width = 0;
        int m_Height = 0;
        bool m_IsComplete = false;
    };
}