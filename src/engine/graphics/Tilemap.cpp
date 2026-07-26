#include "Tilemap.h"
#include "Renderer.h"
#include "utils/Log.h"
#include <fstream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace voidx {
    Tilemap::Tilemap(std::shared_ptr<Texture> tileset, int tileSize, int tilesetColumns)
        : m_Tileset(tileset), m_TileSize(tileSize), m_TilesetColumns(tilesetColumns) {
        if (!m_Tileset) {
            Log::Error("Tilemap", "Tileset texture is null!");
        }
    }

    bool Tilemap::LoadFromTiledJSON(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            Log::Error("Tilemap", "Failed to open map file: " + path);
            return false;
        }

        json root;
        try {
            file >> root;
        } catch (const json::parse_error& e) {
            Log::Error("Tilemap", std::string("JSON parse error: ") + e.what());
            return false;
        }

        m_MapWidth  = root.value("width", 0);
        m_MapHeight = root.value("height", 0);
        if (m_MapWidth <= 0 || m_MapHeight <= 0) {
            Log::Error("Tilemap", "Map has invalid or missing width/height in JSON");
            return false;
        }

        if (m_TilesetColumns <= 0 && root.contains("tilesets") && !root["tilesets"].empty()) {
            const auto& firstTileset = root["tilesets"][0];
            if (firstTileset.contains("columns")) {
                m_TilesetColumns = firstTileset.value("columns", 0);
            } else if (firstTileset.contains("source")) {
                Log::Error("Tilemap", "Tileset is external (.tsx): '" + firstTileset.value("source", "") +
                                       "'. Embed it in Tiled (right-click tileset in the Tilesets panel "
                                       "-> Embed Tileset) or pass tilesetColumns explicitly to the constructor.");
                return false;
            }
        }
        if (m_TilesetColumns <= 0) {
            Log::Error("Tilemap", "Could not determine tileset columns automatically. "
                                   "Pass tilesetColumns explicitly to the Tilemap constructor.");
            return false;
        }

        if (!root.contains("layers") || !root["layers"].is_array()) {
            Log::Error("Tilemap", "No 'layers' array found in map JSON");
            return false;
        }

        m_Layers.clear();
        for (const auto& layerJson : root["layers"]) {
            std::string type = layerJson.value("type", "");
            if (type != "tilelayer") continue;

            std::string encoding = layerJson.value("encoding", "csv");
            if (encoding != "csv") {
                Log::Error("Tilemap", "Layer '" + layerJson.value("name", std::string("")) +
                                       "' uses unsupported encoding '" + encoding +
                                       "'. In Tiled: Layer -> Tile Layer Format -> CSV, then re-export.");
                continue;
            }

            TilemapLayer layer;
            layer.name = layerJson.value("name", "");
            layer.width = layerJson.value("width", m_MapWidth);
            layer.height = layerJson.value("height", m_MapHeight);
            layer.visible = layerJson.value("visible", true);

            if (layerJson.contains("data") && layerJson["data"].is_array()) {
                layer.data.reserve(layerJson["data"].size());
                for (const auto& v : layerJson["data"]) {
                    layer.data.push_back(v.get<int>());
                }
            }

            m_Layers.push_back(std::move(layer));
        }

        if (m_Layers.empty()) {
            Log::Error("Tilemap", "No usable tile layers found (check layer type/encoding above)");
            return false;
        }

        Log::Success("Tilemap", "Loaded map " + std::to_string(m_MapWidth) + "x" + std::to_string(m_MapHeight) +
                                 " with " + std::to_string(m_Layers.size()) + " layer(s), columns=" +
                                 std::to_string(m_TilesetColumns));
        return true;
    }

    void Tilemap::DrawLayerData(const TilemapLayer& layer) {
        if (!m_Tileset || layer.data.empty() || !layer.visible) return;

        float texW = static_cast<float>(m_Tileset->GetWidth());
        float texH = static_cast<float>(m_Tileset->GetHeight());
        float uvW = static_cast<float>(m_TileSize) / texW;
        float uvH = static_cast<float>(m_TileSize) / texH;

        for (int y = 0; y < layer.height; y++) {
            for (int x = 0; x < layer.width; x++) {
                int tileId = layer.data[y * layer.width + x];
                if (tileId == 0) continue;

                int tileIndex = tileId - 1;
                int col = tileIndex % m_TilesetColumns;
                int row = tileIndex / m_TilesetColumns;

                float uvX = static_cast<float>(col) * uvW;
                float uvY = static_cast<float>(row) * uvH;

                glm::vec2 uvs[4] = {
                    {uvX, uvY}, {uvX + uvW, uvY}, {uvX + uvW, uvY + uvH}, {uvX, uvY + uvH}
                };

                glm::vec2 pos(static_cast<float>(x * m_TileSize), static_cast<float>(y * m_TileSize));
                glm::vec2 size(static_cast<float>(m_TileSize), static_cast<float>(m_TileSize));

                Renderer::DrawQuadUV(*m_Tileset, pos, size, uvs, {1, 1, 1, 1});
            }
        }
    }

    void Tilemap::Draw() {
        for (const auto& layer : m_Layers) DrawLayerData(layer);
    }

    void Tilemap::DrawLayer(const std::string& layerName) {
        for (const auto& layer : m_Layers) {
            if (layer.name == layerName) { DrawLayerData(layer); return; }
        }
        Log::Warning("Tilemap", "Layer not found: " + layerName);
    }
}