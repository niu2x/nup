#ifndef NUP_DEMO_H
#define NUP_DEMO_H

#include "nup_app.h"

namespace nup {

class Demo : public AppInterface {
public:
    Demo() { }
    virtual ~Demo() { }
    virtual void frame_callback(double) override;
};

} // namespace nup

#endif