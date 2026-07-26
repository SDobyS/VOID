#pragma once
#include "Texture.h"
#include <string>
#include <stb_truetype.h>

namespace voidx {
    class Font {
    public:
        Font() = default;
        bool Load(const std::string& path, float size = 24.0f);

        Font(const Font&) = delete;
        Font& operator=(const Font&) = delete;

        const Texture& GetTexture() const { return m_Texture; }
        const stbtt_bakedchar* GetGlyphs() const { return m_Glyphs; }

    private:
        Texture m_Texture;
        stbtt_bakedchar m_Glyphs[96];
    };
}