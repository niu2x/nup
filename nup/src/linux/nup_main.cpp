#include <stdlib.h>

#include <exception>
#include <string>

#define GLAD_GL_IMPLEMENTATION
#include "gl_4.3/gl.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "nup_particle.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

static glm::mat4 LookAtMatrix(
    const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
{
    glm::vec3 forward = glm::normalize(target - eye);
    glm::vec3 right = glm::normalize(glm::cross(up, forward));
    glm::vec3 newUp = glm::cross(forward, right);

    glm::mat4 lookAtMatrix(1.0f);
    lookAtMatrix[0] = glm::vec4(right, -glm::dot(right, eye));
    lookAtMatrix[1] = glm::vec4(newUp, -glm::dot(newUp, eye));
    lookAtMatrix[2] = glm::vec4(-forward, glm::dot(forward, eye));
    lookAtMatrix[3] = glm::vec4(0, 0, 0, 1);
    return lookAtMatrix;
}

static glm::mat4 OrthographicProjectionMatrix(
    float l, float r, float b, float t, float n, float f)
{
    glm::mat4 orthoMatrix(1.0f);
    orthoMatrix[0][0] = 2.0f / (r - l);
    orthoMatrix[1][1] = 2.0f / (t - b);
    orthoMatrix[2][2] = -2.0f / (f - n);
    orthoMatrix[3][0] = -(r + l) / (r - l);
    orthoMatrix[3][1] = -(t + b) / (t - b);
    orthoMatrix[3][2] = -(f + n) / (f - n);
    return orthoMatrix;
}

using nup::particle_system::vec3;

int main()
{
    if (!glfwInit()) {
        NUP_ABORT("glfw init failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    auto window_width = 640;
    auto window_height = 480;

    auto native_window
        = glfwCreateWindow(window_width, window_height, "", NULL, NULL);
    glfwMakeContextCurrent(native_window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    auto view = LookAtMatrix(
        glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    auto projection = OrthographicProjectionMatrix(-window_width / 2,
        window_width / 2, -window_height / 2, window_height / 2, -10, 10);
    auto mvp = view * projection;

    nup::particle_system::Config config;

    for (auto i = 0; i < 16; i++) {
        config.mvp[i] = mvp[i / 4][i % 4];
    }

    config.duration = 100;
    config.start_delay = 0;
    config.start_lifetime = 1;
    config.start_size = 10;
    config.start_speed = vec3(100, 200, 0);
    config.start_color = { 1, 1, 1, 0.1 };
    config.final_color = { 0, 0, 1, 0.1 };
    config.gravity_direction = vec3(0, -1, 0);
    config.looping = false;
    config.velocity_over_lifetime = vec3(1, 1, 0);
    config.limit_velocity_over_lifetime = vec3(4, 4, 4);
    config.size_over_lifetime = 64;
    config.max_particles = 100;
    config.gravity = 9.8;

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