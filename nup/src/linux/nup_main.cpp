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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
    glm::vec3 z = glm::normalize(eye - target);
    glm::vec3 x = glm::normalize(glm::cross(up, z));
    glm::vec3 y = glm::cross(z, x);

    glm::mat4 lookAtMatrix(1.0f);
    lookAtMatrix[0] = glm::vec4(x, -glm::dot(x, eye));
    lookAtMatrix[1] = glm::vec4(y, -glm::dot(y, eye));
    lookAtMatrix[2] = glm::vec4(z, -glm::dot(z, eye));
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
    orthoMatrix[3][3] = 1;

    orthoMatrix[0][3] = -(r + l) / (r - l);
    orthoMatrix[1][3] = -(t + b) / (t - b);
    orthoMatrix[2][3] = -(f + n) / (f - n);
    return orthoMatrix;
}

using nup::particle_system::vec3;
static nup::particle_system::Config config;
static bool config_dirty = true;

static const char* font_file
    = "/usr/share/fonts/opentype/noto/NotoSansCJK-Black.ttc";

static void update_mvp(
    nup::particle_system::Config& config, GLFWwindow* native_window)
{

    int display_w, display_h;
    glfwGetFramebufferSize(native_window, &display_w, &display_h);

    auto view = LookAtMatrix(
        glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    auto projection = OrthographicProjectionMatrix(
        -display_w / 2, display_w / 2, -display_h / 2, display_h / 2, -10, 10);
    auto mvp = projection * view;

    for (auto i = 0; i < 16; i++) {
        config.mvp[i] = mvp[i % 4][i / 4];
    }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)width;
    (void)height;
    update_mvp(config, window);
    config_dirty = true;

    glViewport(0, 0, width, height);
}

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

    config.duration = 100;
    config.start_delay = 0;
    config.start_lifetime = 1;
    config.start_size = 10;
    config.start_speed = 0;
    config.start_speed_direction = { 0, 0, 0, 0 };
    config.start_color = { 1, 1, 1, 0.1 };
    config.final_color = { 0, 0, 1, 0.1 };
    config.gravity_direction = { 0, -1, 0, 0 };
    config.looping = false;
    config.velocity_over_lifetime = { 1, 1, 0, 0 };
    config.limit_velocity_over_lifetime = { 4, 4, 4, 0 };
    config.size_over_lifetime = 64;
    config.max_particles = 100;
    config.gravity = 9.8;

    auto system = nup::particle_system::system_create(config);
    if (!system) {
        NUP_ABORT("system create failed");
    }

    glClearColor(0, 0, 0, 0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags
        |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags
        |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Load custom font
    ImFont* font = io.Fonts->AddFontFromFileTTF(font_file, 16.0f);
    if (font == NULL) {
        NUP_ABORT("Failed to load custom font!");
    }
    io.FontDefault = font; // Set the loaded font as default

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    ImGui_ImplGlfw_InitForOpenGL(native_window, true);
    ImGui_ImplOpenGL3_Init("#version 110");

    glfwSetFramebufferSizeCallback(native_window, framebuffer_size_callback);

    auto all_bits
        = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;

    while (!glfwWindowShouldClose(native_window)) {
        glfwPollEvents();
        glClear(all_bits);

        {

            nup::particle_system::system_pre_draw(system);
            nup::particle_system::system_draw(system);
        }

        {

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            {
                ImGui::Begin("Editor");

                if (ImGui::Button("Restart")) {
                    nup::particle_system::system_restart(system);
                }

#define INT_PROTERTY(name, min_value, max_value)                               \
    ImGui::Text(#name ": ");                                                   \
    ImGui::SameLine();                                                         \
    ImGui::DragInt(                                                            \
        "##" #name, (int*)&config.name, 1.0f, min_value, max_value, "%d");

#define FLOAT_PROTERTY(name, min_value, max_value, step)                       \
    ImGui::Text(#name ": ");                                                   \
    ImGui::SameLine();                                                         \
    ImGui::DragFloat(                                                          \
        "##" #name, (float*)&config.name, step, min_value, max_value, "%.3f");

#define FLOAT3_PROTERTY(name, min_value, max_value, step)                      \
    ImGui::Text(#name ": ");                                                   \
    ImGui::SameLine();                                                         \
    ImGui::DragFloat3(                                                         \
        "##" #name, (float*)&config.name, step, min_value, max_value, "%.3f");

                INT_PROTERTY(max_particles, 0, 256);
                FLOAT_PROTERTY(start_size, 0, 256, 0.1);
                FLOAT_PROTERTY(size_over_lifetime, -256, 256, 0.1);
                FLOAT_PROTERTY(start_lifetime, 0, 100, 0.1);
                FLOAT_PROTERTY(gravity, 0, 100, 0.1);
                FLOAT3_PROTERTY(gravity_direction, -1, 1, 0.01);

                FLOAT_PROTERTY(start_speed, 0, 1000, 1);
                FLOAT3_PROTERTY(start_speed_direction, -1, 1, 0.01);

                ImGui::Text("start_color: ");
                ImGui::SameLine();
                ImGui::ColorEdit4(
                    "##start_color", (float*)&config.start_color, 0);

                ImGui::Text("final_color: ");
                ImGui::SameLine();
                ImGui::ColorEdit4(
                    "##final_color", (float*)&config.final_color, 0);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        glfwSwapBuffers(native_window);

        if (config_dirty || true) {
            config_dirty = false;

            nup::particle_system::system_set_config(system, config);
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    nup::particle_system::system_destroy(system);

    glfwDestroyWindow(native_window);
    glfwTerminate();
    return 0;
}