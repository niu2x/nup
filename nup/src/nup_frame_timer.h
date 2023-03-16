#ifndef NUP_FRAME_TIMER_H
#define NUP_FRAME_TIMER_H

#include "nup_type.h"

namespace nup {

class FrameTimer : private noncopyable {
public:
    FrameTimer();
    ~FrameTimer();

    TimeInterval time_delta();

    void reset();

private:
    TimePoint last_;
};

} // namespace nup

#endif