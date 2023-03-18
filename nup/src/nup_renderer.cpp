#include "nup_renderer.h"

namespace nup {

Renderer::Renderer() { }
Renderer::~Renderer() { }

void Renderer::execute(const RendererCommand& cmd)
{
    switch (cmd.type()) {
        case RendererCommandType::CLEAR: {
            clear(cmd.clear_bit_flags());
            break;
        }
        case RendererCommandType::SET_CLEAR_COLOR: {
            set_clear_color(cmd.clear_color());
            break;
        }
    }
}

RendererCommand RendererCommand::clear(BitFlags clear_bit_flags)
{
    RendererCommand cmd;
    cmd.type_ = RendererCommandType::CLEAR;
    cmd.clear_bit_flags_ = &(cmd.bit_flags_[0]);
    *cmd.clear_bit_flags_ = clear_bit_flags;
    return cmd;
}

RendererCommand RendererCommand::set_clear_color(const Color& clear_color)
{
    RendererCommand cmd;
    cmd.type_ = RendererCommandType::SET_CLEAR_COLOR;
    cmd.clear_color_ = &(cmd.colors_[0]);
    *cmd.clear_color_ = clear_color;
    return cmd;
}

} // namespace nup