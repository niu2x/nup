#include "nup_movable_object.h"
#include "nup_transform_utils.h"

namespace nup {

MovableObject::MovableObject()
: parent_(nullptr)
, transform_(TransformUtils::One)
{
}
MovableObject::~MovableObject() { }

void MovableObject::add_child(Ptr<MovableObject> child)
{
    NUP_ASSERT(child->parent_ == nullptr, "child already has parent");

    children_.push_back(child);
    child->parent_ = this;
}

} // namespace nup