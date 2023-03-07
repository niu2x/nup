#ifndef NUP_STREAM_H
#define NUP_STREAM_H

#include "type.h"

namespace nup {

class ReadStream : private noncopyable {
public:
    ReadStream();
    virtual ~ReadStream() = 0;

    // int read(void *);
};

class WriteStream : private noncopyable {
public:
    WriteStream();
    virtual ~WriteStream() = 0;
};

} // namespace nup

#endif