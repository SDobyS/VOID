#pragma once
#include "Texture.h"
#include <memory>
#include <string>
#include <vector>
#include <cstdint>

namespace voidx {
    struct TilesetInfo {
        uint32_t firstgid = 0;
        int tileCount = 0;
        int columns = 0;
        int tileWidth = 32;
        int tileHeight = 32;
        std::shared_ptr<Texture> texture;
    };

    struct TilemapLayer {
        std::string name;
        int width = 0;
        int height = 0;
        std::vector<uint32_t> data;
        bool visible = true;
    };

    struct TilemapObject {
        std::string name;
        std::string type;
        float x = 0.0f;
        float y = 0.0f;
        float width = 0.0f;
        float height = 0.0f;
    };

    class Tilemap {
    public:
        Tilemap() = default;
        Tilemap(std::shared_ptr<Texture> tileset, int tileSize, int tilesetColumns = 0);
        bool LoadFromTiledJSON(const std::string& path);

        void Draw();
        void DrawLayer(const std::string& layerName);
        void SetLayer(float layer) { m_Layer = layer; }

        const std::vector<TilemapObject>& GetObjects() const { return m_Objects; }
        int GetWidth() const { return m_MapWidth; }
        int GetHeight() const { return m_MapHeight; }

    private:
        void DrawLayerData(const TilemapLayer& layer);
        const TilesetInfo* FindTileset(uint32_t gid) const;

        int m_TileSize = 32;
        int m_MapWidth = 0;
        int m_MapHeight = 0;
        float m_Layer = 0.0f;

        std::vector<TilesetInfo> m_Tilesets;
        std::vector<TilemapLayer> m_Layers;
        std::vector<TilemapObject> m_Objects;
    };
}