#ifndef NUP_RENDERER_H
#define NUP_RENDERER_H

#include "nup_type.h"
#include "nup_color.h"

namespace nup {

class Renderer : private noncopyable {
public:
    Renderer();
    virtual ~Renderer() = 0;

    // protected:
    virtual void set_clear_color(const Color& c) = 0;
    virtual void clear() = 0;
};

} // namespace nup

#endif