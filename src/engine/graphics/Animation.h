#pragma once
#include "Texture.h"
#include <glm/glm.hpp>
#include <memory>

namespace voidx {
    class Animation {
    public:
        Animation() = default;
        Animation(std::shared_ptr<Texture> texture, int frameWidth, int frameHeight, int frameCount, float frameTime, int framesPerRow = 1);

        void Update(float dt);
        void GetCurrentUVs(glm::vec2 uv[4]) const;

        void SetFrameTime(float seconds) { if (seconds > 0.0f) m_FrameTime = seconds; }
        float GetFrameTime() const { return m_FrameTime; }

        std::shared_ptr<Texture> GetTexture() const { return m_Texture; }
        int GetCurrentFrame() const { return m_CurrentFrame; }
        int GetFramesPerRow() const { return m_FramesPerRow; }
        int GetFrameWidth() const { return m_FrameWidth; }
        int GetFrameHeight() const { return m_FrameHeight; }

    private:
        std::shared_ptr<Texture> m_Texture;
        int m_FrameWidth = 0;
        int m_FrameHeight = 0;
        int m_FrameCount = 0;
        int m_FramesPerRow = 1;
        float m_FrameTime = 0.1f;
        float m_CurrentTime = 0.0f;
        int m_CurrentFrame = 0;
    };
}