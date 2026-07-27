#include "Animation.h"
#include "utils/Log.h"

namespace voidx {
    Animation::Animation(std::shared_ptr<Texture> texture, int frameWidth, int frameHeight, int frameCount, float frameTime, int framesPerRow)
        : m_Texture(texture), m_FrameWidth(frameWidth), m_FrameHeight(frameHeight),
          m_FrameCount(frameCount > 0 ? frameCount : 1), m_FrameTime(frameTime), m_FramesPerRow(framesPerRow > 0 ? framesPerRow : 1) {
        if (frameCount <= 0) {
            Log::Warning("Animation", "Invalid frameCount provided, defaulting to 1.");
        }
        if (framesPerRow <= 0) {
            Log::Warning("Animation", "Invalid framesPerRow provided, defaulting to 1.");
        }
    }

    void Animation::Update(float dt) {
        m_CurrentTime += dt;
        if (m_CurrentTime >= m_FrameTime) {
            m_CurrentTime = 0.0f;
            m_CurrentFrame = (m_CurrentFrame + 1) % m_FrameCount;
        }
    }

    void Animation::GetCurrentUVs(glm::vec2 uv[4]) const {
        if (!m_Texture) {
            uv[0] = {0,0}; uv[1] = {1,0}; uv[2] = {1,1}; uv[3] = {0,1};
            return;
        }

        float texW = static_cast<float>(m_Texture->GetWidth());
        float texH = static_cast<float>(m_Texture->GetHeight());

        float frameUvW = static_cast<float>(m_FrameWidth) / texW;
        float frameUvH = static_cast<float>(m_FrameHeight) / texH;

        int col = m_CurrentFrame % m_FramesPerRow;
        int row = m_CurrentFrame / m_FramesPerRow;

        float xOffset = static_cast<float>(col) * frameUvW;
        float yOffset = static_cast<float>(row) * frameUvH;

        uv[0] = { xOffset, yOffset };
        uv[1] = { xOffset + frameUvW, yOffset };
        uv[2] = { xOffset + frameUvW, yOffset + frameUvH };
        uv[3] = { xOffset, yOffset + frameUvH };
    }
}