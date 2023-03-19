#ifndef NUP_RENDERABLE_H
#define NUP_RENDERABLE_H

#include "nup_component.h"

namespace nup {

class Renderable : public Component {
public:
    Renderable();
    virtual ~Renderable();
    virtual void step(MovableObject* owner, double ts) override
    {
        unused(ts);
        draw(owner);
    }
    virtual void draw(MovableObject*) = 0;
};

} // namespace nup

#endif