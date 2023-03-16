#ifndef NUP_APP_H
#define NUP_APP_H

namespace nup {

class AppInterface {
public:
    AppInterface() { }
    virtual ~AppInterface() { }
    virtual void frame_callback(double) { }
};

} // namespace nup

#endif