#include "nup_demo.h"
#include "nup_core.h"

namespace nup {

void Demo::frame_callback(double delta)
{
    unused(delta);
    Core::get()->renderer()->set_clear_color(Color::Blue);
    Core::get()->renderer()->clear(BUFFER_ALL);
}

} // namespace nup