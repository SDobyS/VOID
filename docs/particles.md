# Particle System

The particle system uses an **object pool**: inactive particles are reused
instead of being allocated every frame, giving stable performance with no
runtime allocations and predictable memory usage.

## Creating a system

```cpp
ParticleSystem particles(1000); // max capacity
```

## Configuration

```cpp
ParticleConfig config;

config.LifeTime = 1.5f;

config.SpeedMin = 50.0f;
config.SpeedMax = 200.0f;

config.SizeStart = 16.0f;
config.SizeEnd = 0.0f;

config.ColorStart = {1, 1, 0, 1};
config.ColorEnd   = {1, 0, 0, 0};

config.Gravity = {0, 150};

config.DirectionAngle = 0.0f;
config.SpreadAngle = glm::radians(45.0f);

config.RotationSpeedMin = -90.0f;
config.RotationSpeedMax = 90.0f;
```

## Emitting particles

```cpp
particles.Emit({400, 300}, 20, config);
```

| Parameter | Description |
|-----------|-------------|
| Position | Spawn position |
| Count | Number of particles |
| Config | Particle configuration |

## Updating & rendering

```cpp
particles.Update(dt);
particles.Draw();
```

## Statistics

```cpp
uint32_t active   = particles.GetActiveCount();
uint32_t capacity = particles.GetCapacity();
```
