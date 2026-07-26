#pragma once
#include "Texture.h"
#include <memory>
#include <string>
#include <vector>

namespace voidx {
    struct TilemapLayer {
        std::string name;
        int width = 0;
        int height = 0;
        std::vector<int> data;
        bool visible = true;
    };

    class Tilemap {
    public:
        Tilemap() = default;
        Tilemap(std::shared_ptr<Texture> tileset, int tileSize, int tilesetColumns = 0);
        bool LoadFromTiledJSON(const std::string& path);

        void Draw();
        void DrawLayer(const std::string& layerName);

        int GetWidth() const { return m_MapWidth; }
        int GetHeight() const { return m_MapHeight; }

    private:
        void DrawLayerData(const TilemapLayer& layer);

        std::shared_ptr<Texture> m_Tileset;
        int m_TileSize = 32;
        int m_TilesetColumns = 0;

        int m_MapWidth = 0;
        int m_MapHeight = 0;
        std::vector<TilemapLayer> m_Layers;
    };
}