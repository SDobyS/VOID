#include "Framebuffer.h"
#include "utils/Log.h"
#include <utility>

namespace voidx {
    Framebuffer::Framebuffer(int width, int height) : m_Width(width), m_Height(height) {
        Log::Info("Framebuffer", "Creating FBO (" + std::to_string(width) + "x" + std::to_string(height) + ")...");

        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

        glGenRenderbuffers(1, &m_RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        Log::Success("Framebuffer", "Created successfully (FBO ID: " + std::to_string(m_FBO) + ")");
    }

    Framebuffer::~Framebuffer() {
        if (m_FBO) {
            Log::Debug("Framebuffer", "Destroyed (FBO ID: " + std::to_string(m_FBO) + ")");
            glDeleteFramebuffers(1, &m_FBO);
            glDeleteTextures(1, &m_TextureID);
            glDeleteRenderbuffers(1, &m_RBO);
        }
    }

    void Framebuffer::Bind() const {
        Log::Debug("Framebuffer", "Binding FBO ID: " + std::to_string(m_FBO));
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    }

    void Framebuffer::Unbind() const {
        Log::Debug("Framebuffer", "Unbinding FBO (binding default framebuffer)");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Framebuffer::Framebuffer(Framebuffer&& other) noexcept
    : m_FBO(std::exchange(other.m_FBO, 0)),
      m_TextureID(std::exchange(other.m_TextureID, 0)),
      m_RBO(std::exchange(other.m_RBO, 0)),
      m_Width(other.m_Width),
      m_Height(other.m_Height) {
        Log::Debug("Framebuffer", "Move constructor called");
    }

    Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {
        if (this != &other) {
            Log::Debug("Framebuffer", "Move assignment called. Deleting old FBO ID: " + std::to_string(m_FBO));

            if (m_FBO) glDeleteFramebuffers(1, &m_FBO);
            if (m_TextureID) glDeleteTextures(1, &m_TextureID);
            if (m_RBO) glDeleteRenderbuffers(1, &m_RBO);

            m_FBO = std::exchange(other.m_FBO, 0);
            m_TextureID = std::exchange(other.m_TextureID, 0);
            m_RBO = std::exchange(other.m_RBO, 0);
            m_Width = other.m_Width;
            m_Height = other.m_Height;
        }
        return *this;
    }

    void Framebuffer::Resize(int width, int height) {
        if (m_Width == width && m_Height == height) return;

        Log::Info("Framebuffer", "Resizing FBO ID: " + std::to_string(m_FBO) + " to " + std::to_string(width) + "x" + std::to_string(height));

        m_Width = width;
        m_Height = height;

        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    }
}