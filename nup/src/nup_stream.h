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

    Result<char> read_char();

protected:
    virtual Result<size_t> _read(void* buf, size_t n) = 0;
};

class WriteStream : private noncopyable {
public:
    WriteStream();
    virtual ~WriteStream();
    virtual void write(const void* buf, size_t n) = 0;
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

class FileWriteStream : public WriteStream {
public:
    FileWriteStream(Ptr<FileInterface> file);
    virtual ~FileWriteStream();
    virtual void write(const void* buf, size_t n) override;

private:
    Ptr<FileInterface> file_;
};

class StreamFactory : private noncopyable {
public:
    StreamFactory();
    virtual ~StreamFactory() = 0;
};

class FileStreamFactory : StreamFactory {
public:
    FileStreamFactory(Ptr<FileInterfaceFactory> file_factory);
    virtual ~FileStreamFactory();
    Ptr<ReadStream> create_file_read_stream(const String& pathname);
    Ptr<WriteStream> create_file_write_stream(const String& pathname);

private:
    Ptr<FileInterfaceFactory> file_factory_;
};

} // namespace nup

#endif