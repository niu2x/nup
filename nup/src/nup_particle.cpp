#include "nup_particle.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cstddef>

#include "gl_4.3/gl.h"

static const auto* compute_shader_source = R"RAW(

#version 310 es
layout (local_size_x = 256) in;

struct Particle {
    highp vec4 position;
    highp vec4 velocity;
    highp vec4 color;
    highp float size;
    highp float age;
    highp float life;
    highp float alive;
};

layout(std140, binding = 0) buffer Particles {
    Particle particles[];
};

uniform highp float delta_time;
uniform uint particle_count;

void main() {
    uint id = gl_GlobalInvocationID.x;
        Particle p = particles[id];
        // 更新粒子状态
        p.position.xyz = vec3(0.5, 0.5, 0.50);
        particles[id] = p;
}

)RAW";

static const auto* vertex_shader_source = R"RAW(

#version 300 es
layout (location = 0) in highp vec3 inPosition;
layout (location = 1) in highp vec3 inVelocity;
layout (location = 2) in highp vec4 inColor;
layout (location = 3) in highp float inSize;
layout (location = 4) in highp float inAge;
layout (location = 5) in highp float inLife;

out vec4 vColor;

uniform mat4 modelViewProjectionMatrix;

void main() {
    vColor = inColor;
    gl_Position = vec4(inPosition, 1.0);
    gl_PointSize = inSize;
}

)RAW";

static const auto* fragment_shader_source = R"RAW(

#version 300 es
in highp vec4 vColor;
out highp vec4 fragColor;

void main() {
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}

)RAW";

static GLuint compile_shader(GLuint type, const char* source)
{
    GLuint shader = glCreateShader(type);
    if (!shader)
        return 0;

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "compile_shader fail: %s\n", info_log);
        fprintf(stderr, "compile_shader source: %s\n", source);
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
    float alive;
};

struct System {
    Config config;
    GLuint compute_program;
    GLuint draw_program;
    GLuint particle_buffer;
    GLuint location_delta_time;
    GLuint location_particle_count;
    GLuint vao;
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

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    self->vao = vao;

    return true;
}

static bool system_init_gl_program(System* self)
{
    {

        auto compute_shader
            = compile_shader(GL_COMPUTE_SHADER, compute_shader_source);
        if (!compute_shader)
            return false;

        GLuint shaders[] = { compute_shader, 0 };
        auto program = link_program(shaders);

        glDeleteShader(compute_shader);

        if (!program) {
            return false;
        }

        self->compute_program = program;
        self->location_delta_time = glGetUniformLocation(program, "delta_time");
        self->location_particle_count
            = glGetUniformLocation(program, "particle_count");
    }

    {
        auto vertex_shader
            = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
        auto fragment_shader
            = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

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
    return true;
}

static bool system_particle_buffer_init(System* self)
{
    auto particle_count = self->config.max_particles;

    auto* initial_particles
        = (Particle*)calloc(sizeof(Particle), particle_count);
    if (!initial_particles)
        return false;

    for (int i = 0; i < self->config.max_particles; i++) {
        initial_particles[i].alive = false;
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
        goto error_handle;
    }

    if (!system_init_gl_buffer(self)) {
        goto error_handle;
    }

    if (!system_particle_buffer_init(self)) {
        goto error_handle;
    }

    if (!system_init_gl_program(self)) {
        goto error_handle;
    }

    if (!system_init_gl_vao(self)) {
        goto error_handle;
    }

    self->particle_count = 4;

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

        free(self);
    }
}
void system_pre_draw(System* self) { (void)self; }
void system_draw(System* self)
{

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glUseProgram(self->compute_program);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, self->particle_buffer);
    glUniform1f(self->location_delta_time, 1 / 60.0f);
    glUniform1ui(self->location_particle_count, self->particle_count);
    glDispatchCompute((self->particle_count + 255) / 256, 1, 1);

    glUseProgram(self->draw_program);
    glBindVertexArray(self->vao);
    glDrawArrays(GL_POINTS, 0, self->particle_count);
}

} // namespace nup::particle_system