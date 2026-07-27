#include "Font.h"
#include "utils/Log.h"
#include <fstream>
#include <vector>

namespace voidx {
    bool Font::Load(const std::string& path, float size) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            Log::Error("Font load failed", path);
            return false;
        }

        auto fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        m_FontData.resize(fileSize);
        file.read(reinterpret_cast<char*>(m_FontData.data()), fileSize);

        if (!stbtt_InitFont(&m_FontInfo, m_FontData.data(), stbtt_GetFontOffsetForIndex(m_FontData.data(), 0))) {
            Log::Error("Font", "Failed to initialize font info");
            return false;
        }

        m_Scale = stbtt_ScaleForPixelHeight(&m_FontInfo, size);

        int ascent, descent, lineGap;
        stbtt_GetFontVMetrics(&m_FontInfo, &ascent, &descent, &lineGap);
        m_LineHeight = (ascent - descent + lineGap) * m_Scale;

        m_Ranges = {
            {0x0020, 0x007F},
            {0x0400, 0x045F},
            {0x2116, 0x2116}
        };

        uint32_t totalChars = 0;
        for (const auto& r : m_Ranges) totalChars += (r.End - r.Start + 1);
        m_PackedChars.resize(totalChars);

        constexpr int BITMAP_W = 1024;
        constexpr int BITMAP_H = 1024;
        std::vector<unsigned char> bitmap(BITMAP_W * BITMAP_H);

        stbtt_pack_context packCtx;
        if (!stbtt_PackBegin(&packCtx, bitmap.data(), BITMAP_W, BITMAP_H, 0, 1, nullptr)) {
            Log::Error("Font", "Failed to initialize pack context");
            return false;
        }

        std::vector<stbtt_pack_range> packRanges;
        uint32_t offset = 0;
        for (const auto& r : m_Ranges) {
            stbtt_pack_range pr{};
            pr.font_size = size;
            pr.first_unicode_codepoint_in_range = static_cast<int>(r.Start);
            pr.num_chars = static_cast<int>(r.End - r.Start + 1);
            pr.chardata_for_range = &m_PackedChars[offset];
            packRanges.push_back(pr);
            offset += pr.num_chars;
        }

        if (!stbtt_PackFontRanges(&packCtx, m_FontData.data(), 0, packRanges.data(), static_cast<int>(packRanges.size()))) {
            Log::Error("Font", "Failed to pack font ranges (atlas too small?)");
            stbtt_PackEnd(&packCtx);
            return false;
        }

        stbtt_PackEnd(&packCtx);
        m_Texture.LoadFromMemory(bitmap.data(), BITMAP_W, BITMAP_H, 1);
        Log::Success("Font loaded", path);
        return true;
    }

    const stbtt_packedchar* Font::GetCharInfo(uint32_t codepoint) const {
        uint32_t offset = 0;
        for (size_t i = 0; i < m_Ranges.size(); ++i) {
            if (codepoint >= m_Ranges[i].Start && codepoint <= m_Ranges[i].End) {
                return &m_PackedChars[offset + (codepoint - m_Ranges[i].Start)];
            }
            offset += (m_Ranges[i].End - m_Ranges[i].Start + 1);
        }
        return nullptr;
    }

    float Font::GetKerning(uint32_t c1, uint32_t c2, float scale) const {
        return stbtt_GetCodepointKernAdvance(&m_FontInfo, static_cast<int>(c1), static_cast<int>(c2)) * m_Scale * scale;
    }
}