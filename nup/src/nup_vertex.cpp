#include "nup_vertex.h"

namespace nup {

VertexLayout::VertexLayout()
: attributes_nr_(0)
{
}
VertexLayout::~VertexLayout() { }


void VertexLayout::push_attribute(VertexAttribute attr) {
	NUP_ASSERT(attributes_nr_ < NUP_VERTEX_ATTRIBUTE_COUNT, "too many vertex attrs");
    attributes_[attributes_nr_++] = attr;
}


} // namespace nup