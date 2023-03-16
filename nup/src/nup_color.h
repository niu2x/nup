#ifndef NUP_COLOR_H
#define NUP_COLOR_H

#include "nup_type.h"

namespace nup {

class Color {
public:
    union {
        struct {
            uint8_t a;
            uint8_t b;
            uint8_t g;
            uint8_t r;
        };
        uint32_t rgba32;
    };

    NUP_INLINE static Color RGBA(uint8_t r, uint8_t g, uint32_t b, uint32_t a)
    {
        return Color(r, g, b, a);
    }

    NUP_INLINE static Color RGB(uint8_t r, uint8_t g, uint32_t b)
    {
        return Color(r, g, b, 0xFF);
    }

    NUP_INLINE Color(uint8_t r, uint8_t g, uint32_t b, uint32_t a)
    : a(a)
    , b(b)
    , g(g)
    , r(r)
    {
    }

    NUP_INLINE Color(uint32_t rgba32)
    : rgba32(rgba32)
    {
    }

    NUP_INLINE Color(const Color&) = default;
    NUP_INLINE Color& operator=(const Color&) = default;

    ~Color() { }

    static Color White;
    static Color Blue;
};

} // namespace nup

#endif