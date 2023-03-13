#include "nup_file.h"

namespace nup {

FileInterface::FileInterface() { }
FileInterface::~FileInterface() { }

Result<size_t> FileInterface::read(void* buf, size_t n)
{
    NUP_ASSERT(readable(), "file is not readable");
    if (eof()) {
        return E::END_OF_FILE;
    }
    return _read(buf, n);
}

void FileInterface::write(void* buf, size_t n)
{
    NUP_ASSERT(writable(), "file is not writable");
    _write(buf, n);
}

File::File()
: fp_(nullptr)
{
}
File::~File() { close(); }

bool File::open(const String& path, int mode)
{
    NUP_ASSERT(!fp_, "fp_ is not nullptr");
    fp_ = fopen(path.c_str(), mode == O_READ ? "rb" : "wb");
    return fp_ != nullptr;
}

void File::close()
{
    if (fp_) {
        fclose(fp_);
        fp_ = nullptr;
    }
}

bool File::eof() const { return (feof(fp_) || ferror(fp_)); }

void File::seek(int relative, size_t offset)
{
    switch (relative) {
        case S_BEGIN: {
            fseek(fp_, offset, SEEK_SET);
            break;
        }
        case S_CURR: {
            fseek(fp_, offset, SEEK_CUR);
            break;
        }
        case S_END: {
            fseek(fp_, offset, SEEK_END);
            break;
        }
    }
}

size_t File::tell() { return ftell(fp_); }

Result<size_t> File::_read(void* buf, size_t n)
{
    return fread(buf, 1, n, fp_);
}

void File::_write(void* buf, size_t n)
{
    NUP_ASSERT(n == fwrite(buf, 1, n, fp_), "write failed");
}

bool File::readable() const { return mode_ == O_READ; }
bool File::writable() const { return mode_ == O_WRITE; }

Ptr<FileInterface> FileFactory::create_file(const String& path, int mode)
{
    Ptr<File> file = NUP_MAKE_PTR(File);
    return file->open(path, mode) ? file : nullptr;
}

} // namespace nup