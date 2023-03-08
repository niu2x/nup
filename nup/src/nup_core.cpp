#include "nup_core.h"
#include "nup_glfw_window.h"
#include "nup_gl_shader.h"
#include "nup_any.h"
#include "glad_gl.h"

namespace nup {

template <>
Core* Singleton<nup::Core>::instance_ = nullptr;

struct vertex {
    float x, y, z;
    float u, v;
};

static String shader = R"RAW(

#version 430 core
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D imgOutput;

void main() {
    vec4 value = vec4(0.0, 0.0, 0.0, 1.0);
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    value.x = float(texelCoord.x)/(gl_NumWorkGroups.x);
    value.y = float(texelCoord.y)/(gl_NumWorkGroups.y);
    imageStore(imgOutput, texelCoord, value);
}

)RAW";

const String vertex_shader = R"RAW(

#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
    
out vec2 TexCoords;
    
void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}


)RAW";

const String fragment_shader = R"RAW(

#version 430 core
out vec4 FragColor;
    
in vec2 TexCoords;
    
uniform sampler2D tex;
    
void main()
{             
    vec3 texCol = texture(tex, TexCoords).rgb;      
    FragColor = vec4(texCol, 1.0);
}

)RAW";

void Core::run()
{
    auto window = window_factory_->create_window();

    // glClearColor(1, 0, 0, 0);

    // const unsigned int TEXTURE_WIDTH = 512, TEXTURE_HEIGHT = 512;
    // unsigned int texture;
    // glGenTextures(1, &texture);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT,
    // 0,
    //     GL_RGBA, GL_FLOAT, NULL);

    // glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

    // auto computeShader = shader_factory_->create_compute_shader(shader);
    // computeShader->use();

    // glDispatchCompute(
    //     (unsigned int)TEXTURE_WIDTH, (unsigned int)TEXTURE_HEIGHT, 1);

    // auto renderShader
    //     = shader_factory_->create_render_shader(vertex_shader,
    //     fragment_shader);
    // renderShader->use();

    // vertex vertex_array[] = {
    //     { 0, 0, 0, 0, 0 },
    //     { 1, 0, 0, 1, 0 },
    //     { 1, 1, 0, 1, 1 },
    //     { 0, 1, 0, 0, 1 },
    // };

    // GLuint indices[] = { 0, 1, 2, 0, 2, 3 };

    // GLuint vao;
    // glGenVertexArrays(1, &vao);
    // glBindVertexArray(vao);

    // GLuint vbo[2];
    // glGenBuffers(2, vbo);

    // glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    // glBufferData(
    //     GL_ARRAY_BUFFER, sizeof(vertex_array), vertex_array,
    //     GL_DYNAMIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    // glBufferData(
    //     GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    // glEnableVertexAttribArray(0);
    // glEnableVertexAttribArray(1);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), nullptr);
    // glVertexAttribPointer(
    //     1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(float) *
    //     3));

    window->set_frame_callback([window](double) {
        // auto size = window->size();
        // glViewport(0, 0, size.width, size.height);
        // glClear(GL_COLOR_BUFFER_BIT);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    });

    window->run();

    // Any any = to_any(1.0f);
    // printf("any %d %d\n", any.is<double>(), any.is<float>());
}

Core::Core()
{
    window_factory_ = NUP_MAKE_PTR(GlfwWindowFactory);
    shader_factory_ = NUP_MAKE_PTR(GLShaderFactory);
}

Core::~Core()
{
    shader_factory_ = nullptr;
    window_factory_ = nullptr;
}

} // namespace nup