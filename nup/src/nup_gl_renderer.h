#ifndef NUP_GL_RENDERER_H
#define NUP_GL_RENDERER_H

#include "nup_renderer.h"
#include "nup_opengl.h"

namespace nup {

class RendererGL : public Renderer {
public:
    RendererGL();
    ~RendererGL();
    // protected:

    virtual void set_clear_color(const Color& c) override;
    virtual void clear() override;
};

} // namespace nup

#endif