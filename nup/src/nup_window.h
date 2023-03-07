#ifndef NUP_WINDOWS_H
#define NUP_WINDOWS_H

#include "type.h"

namespace nup {

class Window : private boost::noncopyable {
public:
    using FrameCallback = std::function<void(double)>;

    Window();
    virtual ~Window() = 0;

    virtual void set_title(const string& title) = 0;
    virtual void set_size(const IntSize& size) = 0;
    virtual IntSize size() const = 0;

    virtual void run() = 0;
    void set_frame_callback(FrameCallback cb) { frame_callback_ = cb; }

protected:
    std::function<void(double)> frame_callback_;

private:
};

class WindowFactory : private boost::noncopyable {
public:
    WindowFactory();
    virtual ~WindowFactory() = 0;
    virtual Ptr<Window> create_window() = 0;
};

} // namespace nup

#endif
