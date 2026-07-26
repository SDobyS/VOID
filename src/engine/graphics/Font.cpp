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
        std::vector<unsigned char> fileData(fileSize);
        file.read(reinterpret_cast<char*>(fileData.data()), fileSize);

        const int BITMAP_W = 1024;
        const int BITMAP_H = 1024;
        std::vector<unsigned char> bitmap(BITMAP_W * BITMAP_H);

        if (stbtt_BakeFontBitmap(fileData.data(), 0, size, bitmap.data(), BITMAP_W, BITMAP_H, 32, 96, m_Glyphs) <= 0) {
            Log::Error("Font", "Failed to bake font bitmap (atlas too small?)");
            return false;
        }

        std::vector<unsigned char> rgbaBitmap(BITMAP_W * BITMAP_H * 4);
        for (int i = 0; i < BITMAP_W * BITMAP_H; ++i) {
            rgbaBitmap[i * 4 + 0] = 255;
            rgbaBitmap[i * 4 + 1] = 255;
            rgbaBitmap[i * 4 + 2] = 255;
            rgbaBitmap[i * 4 + 3] = bitmap[i];
        }

        m_Texture.LoadFromMemory(rgbaBitmap.data(), BITMAP_W, BITMAP_H, 4);
        Log::Success("Font loaded", path);
        return true;
    }
}