#ifndef NUP_RENDERER_ENUM_H
#define NUP_RENDERER_ENUM_H

namespace nup {

enum {
    BUFFER_COLOR = 1,
    BUFFER_DEPTH = 2,
    BUFFER_STENCIL = 4,
    BUFFER_ALL = BUFFER_COLOR | BUFFER_DEPTH | BUFFER_STENCIL,
};

enum class HardwareBufferType {
    VERTEX,
    INDICE,

    COUNT,
};

enum class HardwareBufferUsage {
    DYNAMIC_DRAW,
    DYNAMIC_READ,
    DYNAMIC_COPY,

    STATIC_DRAW,
    STATIC_READ,
    STATIC_COPY,

    STREAM_DRAW,
    STREAM_READ,
    STREAM_COPY,

    COUNT,
};
}

#endif