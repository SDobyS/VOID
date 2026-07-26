#include "Texture.h"
#include "utils/Log.h"
#include <stb_image.h>

namespace voidx {
    Texture::~Texture() {
        if (m_ID) {
            glDeleteTextures(1, &m_ID);
            Log::Debug("Texture destroyed");
        }
    }

    bool Texture::Load(const std::string& path, TextureFilter filter) {
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
            Log::Success("Texture loaded", path);
            GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            m_Width = width;
            m_Height = height;
            return true;
        }

        Log::Warning("Using fallback texture", path);
        unsigned char fallbackPixels[16] = { 255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 255 };
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, fallbackPixels);
        m_Width = 2; m_Height = 2;
        return true;
    }

    void Texture::LoadFromMemory(unsigned char* data, int width, int height, int channels, TextureFilter filter) {
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
    }

    void Texture::Bind(uint32_t slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }

    void Texture::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
}