#include "Texture.h"
#include "utils/Log.h"
#include <stb_image.h>

Texture::~Texture() {
    if (m_ID) {
        glDeleteTextures(1, &m_ID);
        Log::Debug("Texture destroyed");
    }
}

bool Texture::Load(const std::string& path) {
    stbi_set_flip_vertically_on_load(false);

    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (data) {
        Log::Success("Texture loaded", path);
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        m_Width = width;
        m_Height = height;
        return true;
    }

    // Fallback
    Log::Warning("Using fallback texture", path);
    unsigned char fallbackPixels[16] = {
        255, 000, 255, 255, 000, 000, 000, 255,
        000, 000, 000, 255, 255, 000, 255, 255
    };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, fallbackPixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    m_Width = 2; m_Height = 2;
    return false;
}

void Texture::Bind() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }