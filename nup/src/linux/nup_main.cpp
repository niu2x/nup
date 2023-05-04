#include <stdlib.h>

#include <exception>
#include <string>

#define GLAD_GL_IMPLEMENTATION
#include "gl_4.3/gl.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "nup_particle.h"

class exception : public std::exception {
public:
    exception(const std::string& msg)
    : message_(msg)
    {
    }
    virtual ~exception() { }
    virtual const char* what() const noexcept { return message_.c_str(); }

private:
    std::string message_;
};

#define NUP_ABORT(message) NUP_THROW(exception(message))
#define NUP_THROW(e)       throw e;

using nup::particle_system::vec3;

int main()
{
    if (!glfwInit()) {
        NUP_ABORT("glfw init failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    auto native_window = glfwCreateWindow(640, 480, "", NULL, NULL);
    glfwMakeContextCurrent(native_window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    nup::particle_system::Config config;

    config.duration = 100;
    config.start_delay = 0;
    config.start_lifetime = 1;
    config.start_size = 10;
    config.start_speed = vec3(0, 0, 0);
    config.start_color = { 1, 1, 1, 1 };
    config.gravity_direction = vec3(0, 1, 0);
    config.looping = false;
    config.velocity_over_lifetime = vec3(1, 1, 0);
    config.limit_velocity_over_lifetime = vec3(4, 4, 4);
    config.size_over_lifetime = 1;
    config.max_particles = 100;
    config.gravity = 0.9;

    auto system = nup::particle_system::system_create(config);
    if (!system) {
        NUP_ABORT("system create failed");
    }

    glClearColor(0, 0, 0, 0);
    while (!glfwWindowShouldClose(native_window)) {
        glfwPollEvents();
        glClear(
            GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        nup::particle_system::system_pre_draw(system);
        nup::particle_system::system_draw(system);
        glfwSwapBuffers(native_window);
    }

    nup::particle_system::system_destroy(system);

    glfwDestroyWindow(native_window);
    glfwTerminate();
    return 0;
}