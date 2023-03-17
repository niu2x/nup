#ifndef NUP_VERTEX_H
#define NUP_VERTEX_H

#include "nup_type.h"
#include "nup_renderer_enum.h"

#define NUP_VERTEX_ATTRIBUTE_COUNT NUP_ENUM_COUNT(VertexAttribute)

namespace nup {

class VertexLayout {
public:
    VertexLayout();
    ~VertexLayout();
    void push_attribute(VertexAttribute attr);
private:
    VertexAttribute attributes_[NUP_VERTEX_ATTRIBUTE_COUNT];
    int attributes_nr_;
};

} // namespace nup

#endif