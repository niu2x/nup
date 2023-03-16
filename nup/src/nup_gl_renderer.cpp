#include "nup_gl_renderer.h"

namespace nup {

RendererGL::RendererGL() { }
RendererGL::~RendererGL() { }

void RendererGL::set_clear_color(const Color& c)
{
    glClearColor(c.r, c.g, c.b, c.a);
}
void RendererGL::clear() { glClear(GL_COLOR_BUFFER_BIT); }

} // namespace nup