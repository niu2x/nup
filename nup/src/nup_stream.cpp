#include "nup_stream.h"
#include "nup_core.h"

namespace nup {

ReadStream::ReadStream() { }
ReadStream::~ReadStream() { }

Result<size_t> ReadStream::read(void* buf, size_t n)
{
    if (eof()) {
        return E::END_OF_FILE;
    }
    return _read(buf, n);
}

Result<char> ReadStream::read_char()
{
    char c;
    auto ret = read(&c, 1);
    if (ret) {
        return c;
    }
    return ret;
}

WriteStream::WriteStream() { }
WriteStream::~WriteStream() { }

MemoryReadStream::MemoryReadStream()
: read_pos_(0)
{
}

MemoryReadStream::~MemoryReadStream() { }

Result<size_t> MemoryReadStream::_read(void* buf, size_t n)
{
    size_t available = data_.size() - read_pos_;
    size_t real_read = min(available, n);

    memcpy(buf, (uint8_t*)data_.ptr() + read_pos_, real_read);

    read_pos_ += real_read;
    return real_read;
}

bool MemoryReadStream::eof() const { return read_pos_ >= data_.size(); }

FileReadStream::FileReadStream(Ptr<FileInterface> file)
: file_(file)
{
    NUP_ASSERT(file_->readable(), "is not readable");
}

FileReadStream::~FileReadStream() { }

Result<size_t> FileReadStream::_read(void* buf, size_t n)
{
    return file_->read(buf, n);
}

bool FileReadStream::eof() const { return file_->eof(); }

FileWriteStream::FileWriteStream(Ptr<FileInterface> file)
: file_(file)
{
    NUP_ASSERT(file_->writable(), "is not writable");
}

FileWriteStream::~FileWriteStream() { }

void FileWriteStream::write(const void* buf, size_t n) { file_->write(buf, n); }

FileStreamFactory::FileStreamFactory(Ptr<FileInterfaceFactory> file_factory)
: file_factory_(file_factory)
{
}

FileStreamFactory::~FileStreamFactory() { }

Ptr<ReadStream> FileStreamFactory::create_file_read_stream(
    const String& pathname)
{
    auto file = file_factory_->create_file(pathname);
    return file ? NUP_MAKE_PTR(FileReadStream, file) : nullptr;
}

Ptr<WriteStream> FileStreamFactory::create_file_write_stream(
    const String& pathname)
{
    auto file = file_factory_->create_file(pathname, File::O_WRITE);
    return file ? NUP_MAKE_PTR(FileWriteStream, file) : nullptr;
}

StreamFactory::StreamFactory() { }
StreamFactory::~StreamFactory() { }

} // namespace nup