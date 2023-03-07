#ifndef NUP_H
#define NUP_H

#include "type.h"
#include "nup_window.h"
#include "nup_shader.h"

namespace nup {

class Core : public Singleton<Core> {
public:
    Core();
    ~Core();
    void run();

private:
    Ptr<WindowFactory> window_factory_;
    Ptr<ShaderFactory> shader_factory_;
};

} // namespace nup

#endif