#include "nup_gl_renderer.h"

namespace nup {

RendererGL::RendererGL() { }
RendererGL::~RendererGL() { }

void RendererGL::set_clear_color(const Color& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void RendererGL::clear(BitFlags buffers)
{
    GLuint bits = 0;
    if (buffers & BUFFER_COLOR)
        bits |= GL_COLOR_BUFFER_BIT;
    if (buffers & BUFFER_DEPTH)
        bits |= GL_DEPTH_BUFFER_BIT;
    if (buffers & BUFFER_COLOR)
        bits |= GL_STENCIL_BUFFER_BIT;

    glClear(bits);
}

} // namespace nup