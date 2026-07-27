#pragma once
#include "graphics/Texture.h"
#include "graphics/Shader.h"
#include "graphics/Font.h"
#include <memory>
#include <unordered_map>
#include <string>

namespace voidx {
    class AssetManager {
    public:
        static void Init();
        static void Shutdown();

        static bool Exists(const std::string& name);
        [[nodiscard]] static std::string GetBasePath() { return m_BasePath; }

        static std::shared_ptr<Texture> LoadTexture(const std::string& name, const std::string& path, TextureFilter filter = TextureFilter::Nearest);
        static std::shared_ptr<Texture> GetTexture(const std::string& name);
        static void UnloadTexture(const std::string& name);

        static std::shared_ptr<Font> LoadFont(const std::string& name, const std::string& path, float size);
        static std::shared_ptr<Font> GetFont(const std::string& name);
        static void UnloadFont(const std::string& name);

        static std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
        static std::shared_ptr<Shader> GetShader(const std::string& name);
        static void UnloadShader(const std::string& name);

    private:
        static std::string ResolvePath(const std::string& path);

        static std::string m_BasePath;
        static std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
        static std::unordered_map<std::string, std::weak_ptr<Texture>> m_TexturePaths;
        static std::unordered_map<std::string, std::shared_ptr<Font>> m_Fonts;
        static std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
    };
}