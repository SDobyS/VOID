#pragma once
#include "Sprite.h"
#include "Animation.h"

namespace voidx {
    class AnimatedSprite {
    public:
        AnimatedSprite() = default;
        AnimatedSprite(std::shared_ptr<Texture> texture, int frameWidth, int frameHeight, int frameCount, float frameTime, int framesPerRow = 1);

        void Update(float dt);
        void Draw();

        void SetPosition(const glm::vec2& pos) { m_Sprite.SetPosition(pos); }
        void SetRotation(float rot) { m_Sprite.SetRotation(rot); }
        void SetPivot(const glm::vec2& pivot) { m_Sprite.SetPivot(pivot); }
        void SetFlipX(bool flip) { m_Sprite.SetFlipX(flip); }
        void SetFlipY(bool flip) { m_Sprite.SetFlipY(flip); }
        void SetColor(const glm::vec4& color) { m_Sprite.SetColor(color); }
        void SetFrameTime(float seconds) { m_Animation.SetFrameTime(seconds); }
        glm::vec2 GetPosition() const { return m_Sprite.GetPosition(); }

    private:
        Sprite m_Sprite;
        Animation m_Animation;
    };
}