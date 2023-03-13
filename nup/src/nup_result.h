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

    Result(const Result&) = default;
    Result& operator=(const Result&) = default;

    template <class U>
    Result(const Result<U>& other)
    {
        NUP_ASSERT(!other, "not allowed");
        error_ = other.error();
    }

    ~Result() { }

    operator bool() const { return error_ == E::OK; }

    Error error() const { return error_; }

    const T& operator*() const { return data_; }
    T& operator*() { return data_; }

private:
    T data_;
    Error error_;
};

} // namespace nup

#endif