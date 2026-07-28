#include "ParticleSystem.h"
#include "SceneRenderer.h"

namespace voidx {
    ParticleSystem::ParticleSystem(uint32_t maxParticles) : m_RNG(std::random_device{}()) {
        m_Particles.resize(maxParticles);
        for (auto& p : m_Particles) p.Life = 0.0f;
    }

    void ParticleSystem::Emit(const glm::vec2& position, uint32_t count, const ParticleConfig& config) {
        std::uniform_real_distribution<float> distSpeed(config.SpeedMin, config.SpeedMax);
        std::uniform_real_distribution<float> distAngle(
            config.DirectionAngle - config.SpreadAngle * 0.5f,
            config.DirectionAngle + config.SpreadAngle * 0.5f
        );
        std::uniform_real_distribution<float> distRotSpeed(config.RotationSpeedMin, config.RotationSpeedMax);

        for (uint32_t i = 0; i < count; i++) {
            Particle& p = m_Particles[m_PoolIndex];
            p.Position = position;

            float angle = distAngle(m_RNG);
            float speed = distSpeed(m_RNG);
            p.Velocity = { cos(angle) * speed, sin(angle) * speed };
            p.Gravity = config.Gravity;

            p.ColorStart = config.ColorStart;
            p.ColorEnd = config.ColorEnd;
            p.SizeStart = config.SizeStart;
            p.SizeEnd = config.SizeEnd;

            p.Rotation = 0.0f;
            p.RotationSpeed = distRotSpeed(m_RNG);

            p.LifeTime = config.LifeTime > 0.0f ? config.LifeTime : 0.001f;
            p.Life = p.LifeTime;
            p.ZOrder = config.ZOrder;

            m_PoolIndex = (m_PoolIndex + 1) % m_Particles.size();
        }
    }

    void ParticleSystem::Update(float dt) {
        for (auto& p : m_Particles) {
            if (p.Life > 0.0f) {
                p.Velocity += p.Gravity * dt;
                p.Position += p.Velocity * dt;
                p.Rotation += p.RotationSpeed * dt;
                p.Life -= dt;
            }
        }
    }

    void ParticleSystem::Draw() {
        for (auto& p : m_Particles) {
            if (p.Life > 0.0f) {
                float t = 1.0f - (p.Life / p.LifeTime);
                glm::vec4 color = glm::mix(p.ColorStart, p.ColorEnd, t);
                float size = glm::mix(p.SizeStart, p.SizeEnd, t);

                QuadRenderCommand cmd;
                cmd.TextureID = 0;
                cmd.Position = p.Position;
                cmd.Size = { size, size };
                cmd.Color = color;
                cmd.Rotation = p.Rotation;
                cmd.IsTransparent = true;
                cmd.Pivot = {0.5f, 0.5f};
                cmd.ZOrder = p.ZOrder;
                SceneRenderer::Submit(cmd);
            }
        }
    }

    uint32_t ParticleSystem::GetActiveCount() const {
        uint32_t count = 0;
        for (const auto& p : m_Particles) if (p.Life > 0.0f) count++;
        return count;
    }
}