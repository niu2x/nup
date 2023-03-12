#ifndef NUP_FILE_H
#define NUP_FILE_H

#include "nup_type.h"
#include "nup_result.h"

namespace nup {

class FileInterface : private noncopyable {
public:
    enum {
        S_BEGIN,
        S_END,
        S_CURR,
    };

    FileInterface();
    virtual ~FileInterface() = 0;

    void rewind() { seek(S_BEGIN, 0); }
    Result<size_t> read(void* buf, size_t n);

public:
    virtual bool eof() const = 0;
    virtual void seek(int relative, size_t offset) = 0;
    virtual size_t tell() = 0;

protected:
    virtual Result<size_t> _read(void* buf, size_t n) = 0;
};

class File : public FileInterface {
public:
    File();
    virtual ~File() = 0;

    bool open(const String& path);
    void close();

public:
    virtual bool eof() const override;
    virtual void seek(int relative, size_t offset) override;
    virtual size_t tell() override;

protected:
    virtual Result<size_t> _read(void* buf, size_t n) override;

private:
    FILE* fp_;
};

} // namespace nup

#endif