#include "nup_particle.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cstddef>

#include "gl_4.3/gl.h"

static const auto* common_shader = R"RAW(

layout(std140) uniform Config
{
    highp mat4 mvp;

    highp vec4 velocity_over_lifetime;
    highp vec4 limit_velocity_over_lifetime;
    highp vec4 gravity_direction;
    highp vec4 start_speed;

    highp vec4 start_color;
    highp vec4 final_color;

    highp float duration;
    highp float start_delay;
    highp float start_lifetime;
    highp float size_over_lifetime;
    highp float gravity;
    highp float start_size;

    highp uint max_particles;
    highp int looping;
} config;

)RAW";

static const char* compute_shader_source[] = { R"RAW(

#version 310 es

)RAW",

    common_shader,

    R"RAW(



layout (local_size_x = 256) in;

struct Particle {
    highp vec4 position;
    highp vec4 velocity;
    highp vec4 color;
    highp float size;
    highp float age;
    highp float life;
    highp int alive;
};

layout(std140, binding = 0) buffer Particles {
    Particle particles[];
};

uniform highp float delta_time;

float random(vec2 st) {
    const float a = 12.9898;
    const float b = 78.233;
    const float c = 43758.5453;
    float dt = dot(st.xy, vec2(a,b));
    float sn = mod(dt, 3.14);
    return fract(sin(sn) * c);
}


void main() {
    uint id = gl_GlobalInvocationID.x;

    if(id < config.max_particles) {

        Particle p = particles[id];

        if(p.alive > 0) {
            p.velocity += config.gravity_direction * config.gravity ; 
            p.position += p.velocity * delta_time;
            p.size += config.size_over_lifetime * delta_time;
            p.color = mix(config.start_color, config.final_color, p.age/p.life);

            p.age += delta_time;
            if(p.age >= p.life) {
                p.alive = -1;
                p.age = random(vec2(gl_GlobalInvocationID.xy));
            }
        }
        else {
            p.age -= delta_time;
            if(p.age <= 0.0){
                p.alive = 1;
                p.position = vec4(0.0, 0.0, 0.0, 0.0);
                p.velocity = config.start_speed;
                p.age = 0.0;
                p.size = config.start_size;
                p.life = config.start_lifetime;
                p.color = config.start_color;
            }
        }

        particles[id] = p;

    }
}

)RAW" };

static const char* vertex_shader_source[] = { R"RAW(

#version 300 es

)RAW",
    common_shader,
    R"RAW(
layout (location = 0) in highp vec3 inPosition;
layout (location = 1) in highp vec3 inVelocity;
layout (location = 2) in highp vec4 inColor;
layout (location = 3) in highp float inSize;
layout (location = 4) in highp float inAge;
layout (location = 5) in highp float inLife;
layout (location = 6) in highp int alive;
    
out vec4 vColor;

void main() {
    if(alive < 0) {
        gl_Position = vec4(10.0, 10.0, 10.0, 10.0);
    }
    else {
        vColor = inColor;
        gl_Position = config.mvp * vec4(inPosition, 1.0);
        gl_PointSize = inSize;
    }

}

)RAW" };

static const char* fragment_shader_source[] = { R"RAW(

#version 300 es
in highp vec4 vColor;
out highp vec4 fragColor;

void main() {
    fragColor = vColor;
}

)RAW" };

template <typename T, std::size_t N>
static inline std::size_t array_size(const T (&array)[N])
{
    (void)array;
    return N;
}

static GLuint compile_shader(GLuint type, const char* source[], int count)
{
    GLuint shader = glCreateShader(type);
    if (!shader)
        return 0;

    glShaderSource(shader, count, source, NULL);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "compile_shader fail: %s\n", info_log);
        fprintf(stderr, "compile_shader source: \n");
        for (int i = 0; i < count; i++)
            fprintf(stderr, "%s", source[i]);

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

static GLuint link_program(GLuint shaders[])
{
    GLuint shader_program = glCreateProgram();
    if (!shader_program)
        return 0;

    for (int i = 0; shaders[i] != 0; i++) {
        glAttachShader(shader_program, shaders[i]);
    }

    glLinkProgram(shader_program);

    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(shader_program, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "link error %s\n", info_log);
        glDeleteProgram(shader_program);
        return 0;
    }
    return shader_program;
}

namespace nup::particle_system {

struct Particle {
    Vector position;
    Vector velocity;
    Color color;
    float size;
    float age;
    float life;
    int alive;
};

struct System {
    Config config;
    GLuint compute_program;
    GLuint draw_program;
    GLuint particle_buffer;
    GLuint location_delta_time;
    GLuint vao;
    GLuint uniform_buffer;
    uint32_t particle_count;
};

static bool system_init_gl_vao(System* self)
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, self->particle_buffer);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle),
        (void*)offsetof(Particle, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle),
        (void*)offsetof(Particle, velocity));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle),
        (void*)offsetof(Particle, color));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle),
        (void*)offsetof(Particle, size));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle),
        (void*)offsetof(Particle, age));

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle),
        (void*)offsetof(Particle, life));

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(Particle),
        (void*)offsetof(Particle, alive));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    self->vao = vao;

    return true;
}

static bool system_init_gl_program(System* self)
{
    {
        auto compute_shader = compile_shader(GL_COMPUTE_SHADER,
            compute_shader_source, array_size(compute_shader_source));
        if (!compute_shader)
            return false;

        GLuint shaders[] = { compute_shader, 0 };
        auto program = link_program(shaders);

        glDeleteShader(compute_shader);

        if (!program) {
            return false;
        }

        self->compute_program = program;

#define GET_LOCATION(name)                                                     \
    self->location_##name = glGetUniformLocation(program, #name)
        GET_LOCATION(delta_time);

        auto uniform_block_index = glGetUniformBlockIndex(program, "Config");
        glUniformBlockBinding(program, uniform_block_index, 0);
    }

    {
        auto vertex_shader = compile_shader(GL_VERTEX_SHADER,
            vertex_shader_source, array_size(vertex_shader_source));
        auto fragment_shader = compile_shader(GL_FRAGMENT_SHADER,
            fragment_shader_source, array_size(fragment_shader_source));

        if ((!vertex_shader) || (!fragment_shader)) {
            if (vertex_shader)
                glDeleteShader(vertex_shader);
            if (fragment_shader)
                glDeleteShader(fragment_shader);
            return false;
        }

        GLuint shaders[] = { vertex_shader, fragment_shader, 0 };
        auto program = link_program(shaders);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        if (!program) {
            return false;
        }
        self->draw_program = program;

        auto uniform_block_index = glGetUniformBlockIndex(program, "Config");
        glUniformBlockBinding(program, uniform_block_index, 0);
    }

    return true;
}
static bool system_init_gl_buffer(System* self)
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
        sizeof(Particle) * self->config.max_particles, nullptr,
        GL_DYNAMIC_DRAW);
    self->particle_buffer = buffer;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    GLuint ubo;
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(
        GL_UNIFORM_BUFFER, sizeof(Config), &(self->config), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    self->uniform_buffer = ubo;

    return true;
}

static bool system_particle_buffer_init(System* self)
{
    auto particle_count = self->config.max_particles;

    auto* initial_particles
        = (Particle*)calloc(sizeof(Particle), particle_count);
    if (!initial_particles)
        return false;

    for (auto i = 0U; i < self->config.max_particles; i++) {
        initial_particles[i].alive = 0;
        initial_particles[i].size = 16;
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, self->particle_buffer);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
        sizeof(Particle) * self->config.max_particles, initial_particles);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    free(initial_particles);

    return true;
}

System* system_create(const Config& config)
{
    (void)config;

    auto self = (System*)malloc(sizeof(System));
    if (!self)
        return nullptr;

    memset(self, 0, sizeof(System));

    self->config = config;
    if (!system_init_gl_program(self)) {
        fprintf(stderr, "system_init_gl_program fail\n");
        goto error_handle;
    }

    if (!system_init_gl_buffer(self)) {
        fprintf(stderr, "system_init_gl_buffer fail\n");
        goto error_handle;
    }

    if (!system_particle_buffer_init(self)) {
        fprintf(stderr, "system_particle_buffer_init fail\n");
        goto error_handle;
    }

    if (!system_init_gl_vao(self)) {
        fprintf(stderr, "system_init_gl_vao fail\n");
        goto error_handle;
    }

    return self;

error_handle:
    system_destroy(self);
    return nullptr;
}

void system_destroy(System* self)
{
    if (self) {
        if (self->compute_program)
            glDeleteProgram(self->compute_program);

        if (self->draw_program)
            glDeleteProgram(self->draw_program);

        if (self->particle_buffer)
            glDeleteBuffers(1, &(self->particle_buffer));

        if (self->vao)
            glDeleteVertexArrays(1, &(self->vao));

        if (self->uniform_buffer)
            glDeleteBuffers(1, &(self->uniform_buffer));

        free(self);
    }
}
void system_pre_draw(System* self) { (void)self; }

#define BACKUP_GL_SWITCH(name) auto old_##name = glIsEnabled(name)
#define REVERT_GL_SWITCH(name)                                                 \
    if (old_##name) {                                                          \
        glEnable(name);                                                        \
    } else {                                                                   \
        glDisable(name);                                                       \
    }

void system_draw(System* self)
{
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    BACKUP_GL_SWITCH(GL_VERTEX_PROGRAM_POINT_SIZE);
    BACKUP_GL_SWITCH(GL_BLEND);
    BACKUP_GL_SWITCH(GL_DEPTH_TEST);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glUseProgram(self->compute_program);

    glUniform1f(self->location_delta_time, 1 / 60.0f);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, self->particle_buffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, self->uniform_buffer);
    glDispatchCompute((self->config.max_particles + 255) / 256, 1, 1);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0);

    glUseProgram(self->draw_program);
    glBindVertexArray(self->vao);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, self->uniform_buffer);
    glDrawArrays(GL_POINTS, 0, self->config.max_particles);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0);

    REVERT_GL_SWITCH(GL_DEPTH_TEST);
    REVERT_GL_SWITCH(GL_BLEND);
    REVERT_GL_SWITCH(GL_VERTEX_PROGRAM_POINT_SIZE);
}

} // namespace nup::particle_system