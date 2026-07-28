#pragma once
#include "Renderer.h"
#include <vector>
#include <random>

namespace voidx {
    struct Particle {
        glm::vec2 Position;
        glm::vec2 Velocity;
        glm::vec2 Gravity;

        glm::vec4 ColorStart;
        glm::vec4 ColorEnd;
        float SizeStart;
        float SizeEnd;

        float Rotation;
        float RotationSpeed;
        float LifeTime;
        float Life;
        float ZOrder;
    };

    struct ParticleConfig {
        glm::vec4 ColorStart = { 1.0f, 0.5f, 0.0f, 1.0f };
        glm::vec4 ColorEnd   = { 1.0f, 0.5f, 0.0f, 0.0f };

        float LifeTime = 1.0f;

        float SpeedMin = 50.0f;
        float SpeedMax = 150.0f;

        float SizeStart = 5.0f;
        float SizeEnd = 5.0f;

        glm::vec2 Gravity = { 0.0f, 0.0f };

        float DirectionAngle = 0.0f;
        float SpreadAngle = 6.28318f;

        float RotationSpeedMin = 0.0f;
        float RotationSpeedMax = 0.0f;

        float ZOrder = 0.0f;
    };

    class ParticleSystem {
    public:
        ParticleSystem(uint32_t maxParticles = 1000);
        void Emit(const glm::vec2& position, uint32_t count, const ParticleConfig& config = {});
        void Update(float dt);
        void Draw();

        uint32_t GetActiveCount() const;
        uint32_t GetCapacity() const { return static_cast<uint32_t>(m_Particles.size()); }

    private:
        std::vector<Particle> m_Particles;
        uint32_t m_PoolIndex = 0;
        std::mt19937 m_RNG;
    };
}