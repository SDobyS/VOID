#include "AssetManager.h"
#include "utils/Log.h"
#include <SDL3/SDL.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace voidx {
    std::string AssetManager::m_BasePath;
    std::unordered_map<std::string, std::shared_ptr<Texture>> AssetManager::m_Textures;
    std::unordered_map<std::string, std::weak_ptr<Texture>> AssetManager::m_TexturePaths;
    std::unordered_map<std::string, std::shared_ptr<Font>> AssetManager::m_Fonts;
    std::unordered_map<std::string, std::shared_ptr<Shader>> AssetManager::m_Shaders;

    void AssetManager::Init() {
        m_BasePath = SDL_GetBasePath();
        Log::Info("AssetManager", "Initialized. Base path: " + m_BasePath);
    }

    void AssetManager::Shutdown() {
        m_Textures.clear(); m_TexturePaths.clear(); m_Fonts.clear(); m_Shaders.clear();
        Log::Info("AssetManager", "Shut down (all assets released)");
    }

    std::string AssetManager::ResolvePath(const std::string& path) {
        fs::path p(path);
        return p.is_absolute() ? path : (m_BasePath + path);
    }

    std::shared_ptr<Texture> AssetManager::LoadTexture(const std::string& name, const std::string& path, TextureFilter filter) {
        std::string fullPath = ResolvePath(path);

        fs::path canonicalPath = fs::weakly_canonical(fullPath);
        std::string pathKey = canonicalPath.generic_string();

        auto pathIt = m_TexturePaths.find(pathKey);
        if (pathIt != m_TexturePaths.end()) {
            if (auto tex = pathIt->second.lock()) {
                return tex;
            }
            m_TexturePaths.erase(pathIt);
        }

        auto nameIt = m_Textures.find(name);
        if (nameIt != m_Textures.end()) {
            Log::Warning("AssetManager", "Texture name '" + name + "' already used by a different file. Overwriting.");
            m_Textures.erase(nameIt);
        }

        auto texture = std::make_shared<Texture>();
        if (!texture->Load(fullPath, filter)) {
            Log::Error("AssetManager", "Failed to load texture: " + name);
            return nullptr;
        }

        m_TexturePaths[pathKey] = texture;
        m_Textures[name] = texture;
        return texture;
    }

    std::shared_ptr<Texture> AssetManager::GetTexture(const std::string& name) {
        auto it = m_Textures.find(name);
        if (it != m_Textures.end()) return it->second;
        Log::Warning("AssetManager", "Texture not found: " + name);
        return nullptr;
    }

    void AssetManager::UnloadTexture(const std::string& name) {
        if (auto it = m_Textures.find(name); it != m_Textures.end()) {
            m_Textures.erase(it);
        }
    }

    std::shared_ptr<Font> AssetManager::LoadFont(const std::string& name, const std::string& path, float size) {
        auto it = m_Fonts.find(name);
        if (it != m_Fonts.end()) return it->second;

        auto font = std::make_shared<Font>();
        if (!font->Load(ResolvePath(path), size)) {
            Log::Error("AssetManager", "Failed to load font: " + name);
            return nullptr;
        }
        m_Fonts[name] = font;
        return font;
    }

    std::shared_ptr<Font> AssetManager::GetFont(const std::string& name) {
        auto it = m_Fonts.find(name);
        if (it != m_Fonts.end()) return it->second;
        Log::Warning("AssetManager", "Font not found: " + name);
        return nullptr;
    }

    void AssetManager::UnloadFont(const std::string& name) { m_Fonts.erase(name); }

    std::shared_ptr<Shader> AssetManager::LoadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath) {
        auto it = m_Shaders.find(name);
        if (it != m_Shaders.end()) return it->second;

        auto shader = std::make_shared<Shader>();
        if (!shader->Load(ResolvePath(vertPath), ResolvePath(fragPath))) {
            Log::Error("AssetManager", "Failed to load shader: " + name);
            return nullptr;
        }
        m_Shaders[name] = shader;
        return shader;
    }

    std::shared_ptr<Shader> AssetManager::GetShader(const std::string& name) {
        auto it = m_Shaders.find(name);
        if (it != m_Shaders.end()) return it->second;
        Log::Warning("AssetManager", "Shader not found: " + name);
        return nullptr;
    }

    bool AssetManager::Exists(const std::string& name) {
        return m_Textures.count(name) || m_Fonts.count(name) || m_Shaders.count(name);
    }

    void AssetManager::UnloadShader(const std::string& name) { m_Shaders.erase(name); }
}