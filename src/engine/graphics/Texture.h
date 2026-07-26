#pragma once
#include <glad/gl.h>
#include <string>

namespace voidx {
    enum class TextureFilter {
        Linear,
        Nearest
    };

    class Texture {
    public:
        Texture() = default;
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;
        Texture(Texture&&) = default;
        Texture& operator=(Texture&&) = default;

        bool Load(const std::string& path, TextureFilter filter = TextureFilter::Nearest);
        void LoadFromMemory(unsigned char* data, int width, int height, int channels, TextureFilter filter = TextureFilter::Linear);
        void Bind(uint32_t slot = 0) const;
        void Unbind() const;

        GLuint GetID() const { return m_ID; }
        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }

    private:
        GLuint m_ID = 0;
        int m_Width = 0;
        int m_Height = 0;
    };
}