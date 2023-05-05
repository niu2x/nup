#ifndef NUP_PARTICLE_H
#define NUP_PARTICLE_H

#include <cstdint>

namespace nup::particle_system {

struct Vector {
    float x, y, z;
    float unused;
};

inline Vector vec3(float x, float y, float z) { return { x, y, z, 0 }; }

struct Color {
    float red, green, blue, alpha;
};

struct Config {

    float mvp[16];

    Vector velocity_over_lifetime;
    Vector limit_velocity_over_lifetime;
    Vector gravity_direction;
    Vector start_speed_direction;
    Color start_color;
    Color final_color;

    float duration;
    float start_delay;
    float start_lifetime;
    float size_over_lifetime;
    float gravity;
    float start_size;
    float start_speed;

    uint32_t max_particles;
    int looping;
};

struct System;
System* system_create(const Config& config);
void system_set_config(System* self, const Config& config);
void system_destroy(System* self);
void system_pre_draw(System* self);
void system_draw(System* self);
void system_restart(System* self);

} // namespace nup::particle_system

#endif