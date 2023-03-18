#ifndef NUP_MESH_H
#define NUP_MESH_H

#include "nup_type.h"

namespace nup {

class Mesh {
public:
    struct Vertex {
        RGBA32 color;
        float x, y, z;
        float nx, ny, nz;
        float u, v;
    };

    struct Face {
        uint16_t vertices_[3];
    };

private:
    Vector<Vertex> vertices_;
    Vector<Face> faces_;
};

} // namespace nup

#endif