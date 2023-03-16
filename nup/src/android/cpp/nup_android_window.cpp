#include "nup_android_window.h"
#include <EGL/egl.h>
#include "nup_opengl.h"

namespace nup {

AndroidWindow::AndroidWindow()
{
    {
        NUP_SCOPE_LOCK_DEFAULT;
        if (window_count_++ == 0) {
            gladLoadGLES2(eglGetProcAddress);
        }
    }
}

AndroidWindow::~AndroidWindow()
{
    {
        NUP_SCOPE_LOCK_DEFAULT;
        if (--window_count_ == 0) { }
    }
}

int AndroidWindow::window_count_ = 0;

void AndroidWindow::set_title(const String& title) { unused(title); }
void AndroidWindow::set_size(const IntSize& size) { unused(size); }
IntSize AndroidWindow::size() const { return android_window_size; }

void AndroidWindow::run() { }

} // namespace nup