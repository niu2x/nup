#ifndef NUP_GLFW_WINDOW_H
#define NUP_GLFW_WINDOW_H

#include "nup_window.h"
#include "nup_alloc.h"
struct GLFWwindow;

namespace nup {

class GlfwWindow : public Window {
public:
    GlfwWindow();
    virtual ~GlfwWindow();

    virtual void set_title(const String& title) override;
    virtual void set_size(const IntSize& size) override;
    virtual IntSize size() const override;
    virtual void run() override;

private:
    GLFWwindow* native_window_;
    static int window_count_;
    NUP_MUTEX_DEFAULT;
};

class GlfwWindowFactory : public WindowFactory {
public:
    GlfwWindowFactory();
    virtual ~GlfwWindowFactory();

    virtual Ptr<Window> create_window()
    {
        return Ptr<Window>(NUP_NEW(GlfwWindow));
    }
};

} // namespace nup

#endif