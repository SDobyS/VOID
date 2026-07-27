#pragma once
#include "Texture.h"
#include <string>
#include <stb_truetype.h>
#include <vector>

namespace voidx {
    struct CharRange {
        uint32_t Start;
        uint32_t End;
    };

    class Font {
    public:
        Font() = default;
        bool Load(const std::string& path, float size = 24.0f);

        Font(const Font&) = delete;
        Font& operator=(const Font&) = delete;

        const Texture& GetTexture() const { return m_Texture; }
        const stbtt_packedchar* GetCharInfo(uint32_t codepoint) const;
        float GetKerning(uint32_t c1, uint32_t c2, float scale) const;
        float GetLineHeight() const { return m_LineHeight; }

    private:
        Texture m_Texture;
        stbtt_fontinfo m_FontInfo;
        std::vector<unsigned char> m_FontData;
        std::vector<stbtt_packedchar> m_PackedChars;
        std::vector<CharRange> m_Ranges;
        float m_LineHeight = 0.0f;
        float m_Scale = 1.0f;
    };
}