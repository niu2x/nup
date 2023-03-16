#include "nup_time.h"
#include <chrono>

using namespace std::chrono;

namespace nup {

TimePoint Time::now()
{
    auto now = system_clock::now().time_since_epoch();
    return duration_cast<milliseconds>(now).count();
}

} // namespace nup