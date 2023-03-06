#ifndef NUP_WINDOWS_H
#define NUP_WINDOWS_H

#include "type.h"

namespace nup {

class Window : private boost::noncopyable {
public:
    Window();
    virtual ~Window() = 0;

    virtual void set_title(const string& title) = 0;
    virtual void set_size(const IntSize& size) = 0;

private:
};

class WindowFactory : private boost::noncopyable {
public:
    WindowFactory();
    virtual ~WindowFactory() = 0;
    virtual Ptr<Window> createWindow() = 0;
};

} // namespace nup

#endif
