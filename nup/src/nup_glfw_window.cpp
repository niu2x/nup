#include "nup_glfw_window.h"

namespace nup {

GlfwWindow::GlfwWindow()
: native_window_(nullptr)
{
    native_window_ = glfwCreateWindow(640, 480, "", NULL, NULL);
}

GlfwWindow::~GlfwWindow() { glfwDestroyWindow(window); }

void GlfwWindow::set_title(const string& title) { }

void GlfwWindow::set_size(const IntSize& size) { }