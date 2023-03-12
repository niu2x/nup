#include "nup_file.h"

namespace nup {

FileInterface::FileInterface() { }
FileInterface::~FileInterface() { }

Result<size_t> FileInterface::read(void* buf, size_t n)
{
    if (eof()) {
        return E::END_OF_FILE;
    }
    return _read(buf, n);
}

File::File()
: fp_(nullptr)
{
}
File::~File() { close(); }

bool File::open(const String& path)
{
    NUP_ASSERT(!fp_, "fp_ is not nullptr");
    fp_ = fopen(path.c_str(), "rb");
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

} // namespace nup