#include "nup_window.h"

namespace nup {
Window::Window()
{
    frame_callback_ = [](double) {};
}
Window::~Window() { }

WindowFactory::WindowFactory() { }
WindowFactory::~WindowFactory() { }
} // namespace nup