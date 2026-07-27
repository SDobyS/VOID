#include "Texture.h"
#include "utils/Log.h"
#include <stb_image.h>
#include <utility>

namespace voidx {
    Texture::~Texture() {
        if (m_ID) {
            Log::Debug("Texture", "Destroyed (ID: " + std::to_string(m_ID) + ")");
            glDeleteTextures(1, &m_ID);
        }
    }

    Texture::Texture(Texture&& other) noexcept
    : m_ID(std::exchange(other.m_ID, 0)),
      m_Width(other.m_Width),
      m_Height(other.m_Height) {
        Log::Debug("Texture", "Move constructor called");
    }

    Texture& Texture::operator=(Texture&& other) noexcept {
        if (this != &other) {
            Log::Debug("Texture", "Move assignment called. Deleting old ID: " + std::to_string(m_ID));

            if (m_ID) glDeleteTextures(1, &m_ID);

            m_ID = std::exchange(other.m_ID, 0);
            m_Width = other.m_Width;
            m_Height = other.m_Height;
        }
        return *this;
    }

    bool Texture::Load(const std::string& path, TextureFilter filter) {
        Log::Info("Texture", "Loading texture: " + path);

        stbi_set_flip_vertically_on_load(false);
        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);

        GLenum glFilter = (filter == TextureFilter::Nearest) ? GL_NEAREST : GL_LINEAR;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);

        int width, height, channels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (data) {
            Log::Success("Texture", "Loaded successfully (" + std::to_string(width) + "x" + std::to_string(height) + ", Channels: " + std::to_string(channels) + ")");
            GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            m_Width = width;
            m_Height = height;
            return true;
        }

        Log::Warning("Texture", "Failed to load. Using fallback texture.");
        unsigned char fallbackPixels[16] = { 255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 255 };
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, fallbackPixels);
        m_Width = 2; m_Height = 2;
        return true;
    }

    void Texture::LoadFromMemory(unsigned char* data, int width, int height, int channels, TextureFilter filter) {
        Log::Info("Texture", "Loading texture from memory (" + std::to_string(width) + "x" + std::to_string(height) + ")...");

        if (m_ID) glDeleteTextures(1, &m_ID);
        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);

        GLenum glFilter = (filter == TextureFilter::Nearest) ? GL_NEAREST : GL_LINEAR;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);

        GLenum format = (channels == 4) ? GL_RGBA : GL_RED;
        GLenum internalFormat = (channels == 4) ? GL_RGBA8 : GL_R8;
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        m_Width = width;
        m_Height = height;

        Log::Success("Texture", "Memory texture created (ID: " + std::to_string(m_ID) + ")");
    }

    void Texture::Bind(uint32_t slot) const {
        Log::Debug("Texture", "Binding ID: " + std::to_string(m_ID) + " to slot " + std::to_string(slot));
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }

    void Texture::Unbind() const {
        Log::Debug("Texture", "Unbinding texture (slot 0)");
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}