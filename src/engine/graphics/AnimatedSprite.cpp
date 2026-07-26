#include "AnimatedSprite.h"
#include "Renderer.h"

namespace voidx {

    AnimatedSprite::AnimatedSprite(std::shared_ptr<Texture> texture, int frameWidth, int frameHeight, int frameCount, float frameTime, int framesPerRow)
        : m_Animation(texture, frameWidth, frameHeight, frameCount, frameTime, framesPerRow) {
        m_Sprite = Sprite(texture, {0, 0}, {static_cast<float>(frameWidth), static_cast<float>(frameHeight)});
    }

    void AnimatedSprite::Update(float dt) {
        m_Animation.Update(dt);
    }

    void AnimatedSprite::Draw() {
        auto tex = m_Animation.GetTexture();
        if (!tex) return;

        glm::vec2 uvs[4];
        m_Animation.GetCurrentUVs(uvs);

        glm::vec2 size(
            static_cast<float>(m_Animation.GetFrameWidth()),
            static_cast<float>(m_Animation.GetFrameHeight())
        );

        Renderer::DrawQuadUV(
            *tex,
            m_Sprite.GetPosition(),
            size,
            uvs,
            m_Sprite.GetColor(),
            m_Sprite.GetPivot(),
            m_Sprite.GetRotation(),
            m_Sprite.GetFlipX(),
            m_Sprite.GetFlipY()
        );
    }
}