#include "Tilemap.h"
#include "Renderer.h"
#include "AssetManager.h"
#include "utils/Log.h"
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>
#include "SceneRenderer.h"
#include <filesystem>

namespace fs = std::filesystem;
using json = nlohmann::json;

namespace voidx {
    Tilemap::Tilemap(std::shared_ptr<Texture> tileset, int tileSize, int tilesetColumns)
        : m_TileSize(tileSize) {
        if (tileset) {
            TilesetInfo info;
            info.firstgid = 1;
            info.columns = tilesetColumns > 0 ? tilesetColumns : (tileset->GetWidth() / tileSize);
            info.tileWidth = tileSize;
            info.tileHeight = tileSize;
            info.tileCount = info.columns * (tileset->GetHeight() / tileSize);
            info.texture = tileset;
            m_Tilesets.push_back(info);
        }
    }

    const TilesetInfo* Tilemap::FindTileset(uint32_t gid) const {
        if (m_Tilesets.empty()) return nullptr;
        auto it = std::upper_bound(m_Tilesets.begin(), m_Tilesets.end(), gid,
            [](uint32_t val, const TilesetInfo& ts) {
                return val < ts.firstgid;
            });
        if (it != m_Tilesets.begin()) {
            return &*(--it);
        }
        return nullptr;
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

        fs::path mapPath(path);
        fs::path mapDir = mapPath.parent_path();

        if (root.contains("tilesets") && !root["tilesets"].empty()) {
            m_Tilesets.clear();
            for (const auto& tsJson : root["tilesets"]) {
                TilesetInfo info;
                info.firstgid = tsJson.value("firstgid", 1);
                info.columns = tsJson.value("columns", 0);
                info.tileWidth = tsJson.value("tilewidth", m_TileSize);
                info.tileHeight = tsJson.value("tileheight", m_TileSize);
                info.tileCount = tsJson.value("tilecount", 0);

                if (tsJson.contains("image")) {
                    std::string imgPath = tsJson.value("image", "");
                    fs::path fullPath = fs::weakly_canonical(mapDir / imgPath);
                    std::string name = fullPath.filename().string();

                    info.texture = AssetManager::LoadTexture(name, fullPath.generic_string());
                } else if (tsJson.contains("source")) {
                    Log::Error("Tilemap", "External TSX tilesets are unsupported. Embed tilesets in Tiled.");
                    continue;
                }

                if (info.texture && info.columns > 0) {
                    m_Tilesets.push_back(info);
                }
            }
            std::sort(m_Tilesets.begin(), m_Tilesets.end(), [](const TilesetInfo& a, const TilesetInfo& b){
                return a.firstgid < b.firstgid;
            });
        }

        if (m_Tilesets.empty()) {
            Log::Error("Tilemap", "No valid tilesets found.");
            return false;
        }

        m_Layers.clear();
        m_Objects.clear();

        for (const auto& layerJson : root["layers"]) {
            std::string type = layerJson.value("type", "");

            if (type == "tilelayer") {
                std::string encoding = layerJson.value("encoding", "csv");
                if (encoding != "csv") {
                    Log::Error("Tilemap", "Layer '" + layerJson.value("name", "") + "' uses unsupported encoding. Use CSV.");
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
                        layer.data.push_back(v.get<uint32_t>());
                    }
                }
                m_Layers.push_back(std::move(layer));
            } else if (type == "objectgroup") {
                if (layerJson.contains("objects")) {
                    for (const auto& obj : layerJson["objects"]) {
                        TilemapObject o;
                        o.name = obj.value("name", "");
                        o.type = obj.value("type", "");
                        o.x = obj.value("x", 0.0f);
                        o.y = obj.value("y", 0.0f);
                        o.width = obj.value("width", 0.0f);
                        o.height = obj.value("height", 0.0f);
                        m_Objects.push_back(o);
                    }
                }
            }
        }

        if (m_Layers.empty()) {
            Log::Error("Tilemap", "No usable tile layers found.");
            return false;
        }

        Log::Success("Tilemap", "Loaded map with " + std::to_string(m_Tilesets.size()) + " tilesets, " + std::to_string(m_Layers.size()) + " layers, " + std::to_string(m_Objects.size()) + " objects.");
        return true;
    }

    void Tilemap::DrawLayerData(const TilemapLayer& layer) {
        if (layer.data.empty() || !layer.visible) return;

        for (int y = 0; y < layer.height; y++) {
            for (int x = 0; x < layer.width; x++) {
                uint32_t gid = layer.data[y * layer.width + x];
                if (gid == 0) continue;

                uint32_t maskedGid = gid & 0x0FFFFFFF;
                const TilesetInfo* ts = FindTileset(maskedGid);
                if (!ts || !ts->texture) continue;

                uint32_t localId = maskedGid - ts->firstgid;
                int col = localId % ts->columns;
                int row = localId / ts->columns;

                float texW = static_cast<float>(ts->texture->GetWidth());
                float texH = static_cast<float>(ts->texture->GetHeight());
                float uvW = static_cast<float>(ts->tileWidth) / texW;
                float uvH = static_cast<float>(ts->tileHeight) / texH;

                float uvX = static_cast<float>(col) * uvW;
                float uvY = static_cast<float>(row) * uvH;

                glm::vec2 uvs[4] = {
                    {uvX, uvY}, {uvX + uvW, uvY}, {uvX + uvW, uvY + uvH}, {uvX, uvY + uvH}
                };

                glm::vec2 pos(static_cast<float>(x * ts->tileWidth), static_cast<float>(y * ts->tileHeight));
                glm::vec2 size(static_cast<float>(ts->tileWidth), static_cast<float>(ts->tileHeight));

                QuadRenderCommand cmd;
                cmd.TextureID = ts->texture->GetID();
                cmd.Position = pos;
                cmd.Size = size;
                cmd.UV[0] = uvs[0]; cmd.UV[1] = uvs[1]; cmd.UV[2] = uvs[2]; cmd.UV[3] = uvs[3];
                cmd.Color = {1, 1, 1, 1};
                cmd.UseUVs = true;
                cmd.Pivot = {0.0f, 0.0f};
                cmd.ZOrder = m_Layer;
                SceneRenderer::Submit(cmd);
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