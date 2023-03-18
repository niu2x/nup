#ifndef NUP_RENDERER_H
#define NUP_RENDERER_H

#include "nup_type.h"
#include "nup_color.h"
#include "nup_renderer_enum.h"

namespace nup {

class RendererCommand {
public:
    static RendererCommand clear(BitFlags clear_bit_flags);
    static RendererCommand set_clear_color(const Color& c);

    RendererCommand(const RendererCommand&) = default;
    RendererCommand& operator=(const RendererCommand&) = default;

    ~RendererCommand() { }

    NUP_INLINE RendererCommandType type() const { return type_; }

    NUP_INLINE BitFlags clear_bit_flags() const { return *clear_bit_flags_; }

    NUP_INLINE const Color& clear_color() const { return *clear_color_; };

private:
    RendererCommand() { }

    RendererCommandType type_;
    struct {
        BitFlags bit_flags_[1];
        Color colors_[1];
    };

    union {
        struct {
            BitFlags* clear_bit_flags_;
        };
        struct {
            Color* clear_color_;
        };
    };
};

// 执行对渲染器状态改变的一些命令

class Renderer : private noncopyable {
public:
    Renderer();
    virtual ~Renderer() = 0;

    // protected:
    virtual void set_clear_color(const Color& c) = 0;
    virtual void clear(BitFlags buffers) = 0;

    void execute(const RendererCommand& cmd);
};

} // namespace nup

#endif