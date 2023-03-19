#ifndef NUP_COMPONENT_H
#define NUP_COMPONENT_H

#include "nup_type.h"

namespace nup {

class MovableObject;

class Component : private noncopyable {
public:
    Component() { }
    virtual ~Component() { }
    virtual void step(MovableObject* owner, double ts) = 0;
};

} // namespace nup

#endif