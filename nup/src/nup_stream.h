#ifndef NUP_STREAM_H
#define NUP_STREAM_H

#include "type.h"

namespace nup {

class ReadStream : private noncopyable {
public:
    ReadStream();
    virtual ~ReadStream() = 0;
    // int read(void *);
    virtual int read(void* buf, int n) = 0;
    virtual bool eof() const = 0;
};

class WriteStream : private noncopyable {
public:
    WriteStream();
    virtual ~WriteStream() = 0;
};

class MemoryReadStream : public ReadStream {
public:
    MemoryReadStream();
    virtual ~MemoryReadStream();
};

} // namespace nup

#endif