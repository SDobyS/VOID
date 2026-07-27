#pragma once
#include <glad/gl.h>
#include <string>

namespace voidx {
    enum class TextureFilter {
        Nearest,
        Linear
    };

    class Texture {
    public:
        Texture() = default;
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;

        bool Load(const std::string& path, TextureFilter filter = TextureFilter::Nearest, bool genMipmaps = false);
        void LoadFromMemory(unsigned char* data, int width, int height, int channels, TextureFilter filter = TextureFilter::Linear, bool genMipmaps = false);

        void Bind(uint32_t slot = 0) const;
        void Unbind() const;

        [[nodiscard]] int GetWidth() const { return m_Width; }
        [[nodiscard]] int GetHeight() const { return m_Height; }
        [[nodiscard]] GLuint GetID() const { return m_ID; }

    private:
        GLuint m_ID = 0;
        int m_Width = 0;
        int m_Height = 0;
    };
}