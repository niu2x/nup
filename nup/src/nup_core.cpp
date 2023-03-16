#include "nup_core.h"
#include "nup_config.h"

#if defined(NUP_USE_GLFW)
    #include "nup_glfw_window.h"
#endif

#include "nup_gl_shader.h"
#include "nup_any.h"
#include "nup_opengl.h"

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
    window_->set_frame_callback([this](double ts) { this->step(ts); });
    window_->run();
}

void Core::setup() { window_ = window_factory_->create_window(); }

void Core::step(double ts) { app_->frame_callback(ts); }

void Core::cleanup() { window_ = nullptr; }

Core::Core()
{
#if defined(NUP_USE_GLFW)
    window_factory_ = NUP_MAKE_PTR(GlfwWindowFactory);
#endif

    shader_factory_ = NUP_MAKE_PTR(GLShaderFactory);
    file_factory_ = NUP_MAKE_PTR(FileFactory);
    file_stream_factory_ = NUP_MAKE_PTR(FileStreamFactory, file_factory_);

    app_ = nullptr;
}

Core::~Core()
{
    app_ = nullptr;

    file_stream_factory_ = nullptr;
    file_factory_ = nullptr;
    shader_factory_ = nullptr;
    window_factory_ = nullptr;
}

} // namespace nup