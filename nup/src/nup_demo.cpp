#include "nup_demo.h"
#include "nup_type.h"
#include "nup_opengl.h"

namespace nup {

void Demo::frame_callback(double delta)
{
    glClear(GL_COLOR_BUFFER_BIT);
    unused(delta);
}

} // namespace nup