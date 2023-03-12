#include "nup_stream.h"

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
}

FileReadStream::~FileReadStream() { }

Result<size_t> FileReadStream::_read(void* buf, size_t n)
{
    return file_->read(buf, n);
}

bool FileReadStream::eof() const { return file_->eof(); }

} // namespace nup