#include "nup_frame_timer.h"
#include "nup_time.h"

namespace nup {

FrameTimer::FrameTimer()
: last_(0)
{
    reset();
}

FrameTimer::~FrameTimer() { }

void FrameTimer::reset() { last_ = Time::now(); }

TimeInterval FrameTimer::time_delta()
{
    auto now = Time::now();
    auto delta = now - last_;
    last_ = now;
    return delta;
}

} // namespace nup