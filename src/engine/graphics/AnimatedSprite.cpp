#include "AnimatedSprite.h"
#include "SceneRenderer.h"

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

        QuadRenderCommand cmd;
        cmd.TextureID = tex->GetID();
        cmd.Position = m_Sprite.GetPosition();
        cmd.Size = size;
        cmd.UV[0] = uvs[0]; cmd.UV[1] = uvs[1]; cmd.UV[2] = uvs[2]; cmd.UV[3] = uvs[3];
        cmd.Color = m_Sprite.GetColor();
        cmd.Pivot = m_Sprite.GetPivot();
        cmd.Rotation = m_Sprite.GetRotation();
        cmd.FlipX = m_Sprite.GetFlipX();
        cmd.FlipY = m_Sprite.GetFlipY();
        cmd.UseUVs = true;
        SceneRenderer::Submit(cmd);
    }
}