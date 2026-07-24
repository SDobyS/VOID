#include "Texture.h"
#include "../core/Config.h"
#include "../utils/Log.h"
#include <stb_image.h>
#include <iostream>

Texture::Texture(const std::string& path) {
    // Flip texture
    stbi_set_flip_vertically_on_load(Config::FlipTexture);

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /*
        Default Texture:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        Pixel Texture:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    */

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int nrChannels;
    unsigned char* data = stbi_load(
        path.c_str(),
        &m_Width,
        &m_Height,
        &nrChannels,
        0
    );

    if (data) {
        Log::Success("Texture loaded", path);

        // Format RGB or RGBA
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            m_Width,
            m_Height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data
        );

        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        Log::Warning("Using fallback texture", path);

        unsigned char fallbackPixels[4 * 4] = {
            255, 000, 255, 255, // 0 0 purple
            000, 000, 000, 255, // 1 0 black
            000, 000, 000, 255, // 0 1 black
            255, 000, 255, 255  // 1 1 purple
        };

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            2,
            2,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            fallbackPixels
        );

        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

Texture::~Texture() { if (m_RendererID) glDeleteTextures(1, &m_RendererID); }

void Texture::Bind(GLuint unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }