#ifndef NUP_GLFW_WINDOW_H
#define NUP_GLFW_WINDOW_H

#include "nup_window.h"
#include "nup_alloc.h"

namespace nup {

class GlfwWindow : public Window {
public:
    GlfwWindow();
    virtual ~GlfwWindow();

    virtual void set_title(const string& title) override;
    virtual void set_size(const IntSize& size) override;

private:
    GLFWwindow* native_window_;
    static int window_count_;
};

class GlfwWindowFactory : public WindowFactory {
public:
    GlfwWindowFactory();
    virtual ~GlfwWindowFactory();

    virtual Ptr<Window> createWindow() { return NUP_NEW_T(GlfwWindow); }
};

} // namespace nup

#endif