#ifndef NUP_SCENE_MANAGER_H
#define NUP_SCENE_MANAGER_H

#include "nup_type.h"
#include "nup_movable_object.h"

namespace nup {

class SceneManager : private noncopyable {
public:
    SceneManager();
    virtual ~SceneManager() = 0;
    virtual void step(double ts) = 0;

private:
    Ptr<MovableObject> root_;
};

} // namespace nup

#endif