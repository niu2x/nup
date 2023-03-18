#include "nup_demo.h"
#include "nup_core.h"

namespace nup {

void Demo::frame_callback(double delta)
{
    unused(delta);

    static auto clear_cmd = RendererCommand::clear(BUFFER_ALL);
    static auto set_clear_color_cmd
        = RendererCommand::set_clear_color(Color::White);

    Core::get()->renderer()->execute(set_clear_color_cmd);
    Core::get()->renderer()->execute(clear_cmd);
}

} // namespace nup