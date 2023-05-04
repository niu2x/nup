#ifndef NUP_PARTICLE_H
#define NUP_PARTICLE_H

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
    float duration;

    float start_delay;
    float start_lifetime;
    float start_size;
    Vector start_speed;
    Color start_color;

    Vector gravity_direction;
    bool looping;

    Vector velocity_over_lifetime;
    Vector limit_velocity_over_lifetime;
    float size_over_lifetime;

    int max_particles;
};

struct System;
System* system_create(const Config& config);
void system_destroy(System* self);
void system_pre_draw(System* self);
void system_draw(System* self);

} // namespace nup::particle_system

#endif