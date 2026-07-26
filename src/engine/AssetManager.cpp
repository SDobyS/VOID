#include "AssetManager.h"
#include "utils/Log.h"
#include <SDL3/SDL.h>

namespace voidx {
    std::string AssetManager::m_BasePath;
    std::unordered_map<std::string, std::shared_ptr<Texture>> AssetManager::m_Textures;
    std::unordered_map<std::string, std::shared_ptr<Font>> AssetManager::m_Fonts;
    std::unordered_map<std::string, std::shared_ptr<Shader>> AssetManager::m_Shaders;

    void AssetManager::Init() {
        m_BasePath = SDL_GetBasePath();
        Log::Info("AssetManager", "Initialized. Base path: " + m_BasePath);
    }

    void AssetManager::Shutdown() {
        m_Textures.clear(); m_Fonts.clear(); m_Shaders.clear();
        Log::Info("AssetManager", "Shut down (all assets released)");
    }

    std::shared_ptr<Texture> AssetManager::LoadTexture(const std::string& name, const std::string& path, TextureFilter filter) {
        auto it = m_Textures.find(name);
        if (it != m_Textures.end()) return it->second;

        auto texture = std::make_shared<Texture>();
        if (!texture->Load(m_BasePath + path, filter)) {
            Log::Error("AssetManager", "Failed to load texture: " + name);
            return nullptr;
        }
        m_Textures[name] = texture;
        return texture;
    }

    std::shared_ptr<Texture> AssetManager::GetTexture(const std::string& name) {
        auto it = m_Textures.find(name);
        if (it != m_Textures.end()) return it->second;
        Log::Warning("AssetManager", "Texture not found: " + name);
        return nullptr;
    }

    void AssetManager::UnloadTexture(const std::string& name) { m_Textures.erase(name); }

    std::shared_ptr<Font> AssetManager::LoadFont(const std::string& name, const std::string& path, float size) {
        auto it = m_Fonts.find(name);
        if (it != m_Fonts.end()) return it->second;

        auto font = std::make_shared<Font>();
        if (!font->Load(m_BasePath + path, size)) {
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
        if (!shader->Load(m_BasePath + vertPath, m_BasePath + fragPath)) {
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