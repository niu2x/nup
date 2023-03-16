#ifndef NUP_ANDROID_WINDOW_H
#define NUP_ANDROID_WINDOW_H

#include "nup_window.h"
#include "nup_alloc.h"

namespace nup {

extern IntSize android_window_size;

class AndroidWindow : public Window {
public:
    AndroidWindow();
    virtual ~AndroidWindow();

    virtual void set_title(const String& title) override;
    virtual void set_size(const IntSize& size) override;
    virtual IntSize size() const override;
    virtual void run() override;

private:
    static int window_count_;
    NUP_MUTEX_DEFAULT;
};

class AndroidWindowFactory : public WindowFactory {
public:
    AndroidWindowFactory() { }
    virtual ~AndroidWindowFactory() { }
    virtual Ptr<Window> create_window()
    {
        return Ptr<Window>(NUP_NEW(AndroidWindow));
    }
};

} // namespace nup

#endif