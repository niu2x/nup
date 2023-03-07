#include "nup_core.h"
#include "nup_glfw_window.h"

namespace nup {

template <>
Core* Singleton<nup::Core>::instance_ = nullptr;

void Core::run()
{
    auto window = window_factory_->create_window();
    window->run();
}

Core::Core() { window_factory_ = NUP_MAKE_PTR(GlfwWindowFactory); }

Core::~Core() { window_factory_ = nullptr; }

} // namespace nup