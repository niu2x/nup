#ifndef NUP_TIME_H
#define NUP_TIME_H

#include "nup_type.h"

namespace nup {

struct Time {
    static TimePoint now();
};

} // namespace nup

#endif