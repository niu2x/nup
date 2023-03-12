#ifndef NUP_STREAM_H
#define NUP_STREAM_H

#include "nup_type.h"
#include "nup_data.h"
#include "nup_result.h"
#include "nup_file.h"

namespace nup {

class ReadStream : private noncopyable {
public:
    ReadStream();
    virtual ~ReadStream() = 0;
    // int read(void *);
    Result<size_t> read(void* buf, size_t n);
    virtual bool eof() const = 0;

protected:
    virtual Result<size_t> _read(void* buf, size_t n) = 0;
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

    virtual bool eof() const override;

protected:
    virtual Result<size_t> _read(void* buf, size_t n) override;

private:
    Data data_;
    size_t read_pos_;
};

class FileReadStream : public ReadStream {
public:
    FileReadStream(Ptr<FileInterface> file);
    virtual ~FileReadStream();

    virtual bool eof() const override;

protected:
    virtual Result<size_t> _read(void* buf, size_t n) override;

private:
    Ptr<FileInterface> file_;
};

} // namespace nup

#endif