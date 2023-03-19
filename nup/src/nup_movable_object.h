#ifndef NUP_MOVABLE_OBJECT_H
#define NUP_MOVABLE_OBJECT_H

#include "nup_type.h"
#include "nup_component.h"

namespace nup {

class MovableObject : private noncopyable {
public:
    MovableObject();
    virtual ~MovableObject();
    void add_child(Ptr<MovableObject> child);

private:
    MovableObject* parent_;
    Vector<Ptr<MovableObject>> children_;
    Vector<Ptr<Component>> components_;
    Transform transform_;
};

} // namespace nup

#endif