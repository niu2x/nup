#ifndef NUP_RESULT_H
#define NUP_RESULT_H

#include "nup_type.h"

namespace nup {

template <class T>
class Result {
public:
    Result(const T& t)
    : data_(t)
    , error_(E::OK)
    {
    }
    Result(Error e)
    : error_(e)
    {
    }
    ~Result() { }

private:
    T data_;
    Error error_;
};

} // namespace nup

#endif