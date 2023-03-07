#include "nup_glfw_window.h"

#define GLAD_GL_IMPLEMENTATION
#include "glad_gl.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace nup {

GlfwWindow::GlfwWindow()
: native_window_(nullptr)
{
    {
        NUP_SCOPE_LOCK_DEFAULT;
        if (window_count_++ == 0) {
            if (!glfwInit())
                NUP_ABORT("glfw init failed");
        }
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    native_window_ = glfwCreateWindow(640, 480, "", NULL, NULL);

    glfwMakeContextCurrent(native_window_);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
}

void GlfwWindow::run()
{
    while (!glfwWindowShouldClose(native_window_)) {
        glfwPollEvents();
        // int width, height;

        // glfwGetFramebufferSize(native_window_, &width, &height);
        // glViewport(0, 0, width, height);
        // glClear(GL_COLOR_BUFFER_BIT);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(native_window_);
    }
}

GlfwWindow::~GlfwWindow()
{
    glfwDestroyWindow(native_window_);

    {
        NUP_SCOPE_LOCK_DEFAULT;
        if (--window_count_ == 0) {
            glfwTerminate();
        }
    }
}

void GlfwWindow::set_title(const string& title) { unused(title); }

void GlfwWindow::set_size(const IntSize& size) { unused(size); }

int GlfwWindow::window_count_ = 0;

GlfwWindowFactory::GlfwWindowFactory() { }
GlfwWindowFactory::~GlfwWindowFactory() { }

} // namespace nup