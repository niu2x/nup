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
    void write(void* buf, size_t n);

public:
    virtual bool eof() const = 0;
    virtual void seek(int relative, size_t offset) = 0;
    virtual size_t tell() = 0;
    virtual bool readable() const = 0;
    virtual bool writable() const = 0;

protected:
    virtual Result<size_t> _read(void* buf, size_t n) = 0;
    virtual void _write(void* buf, size_t n) = 0;
};

class File : public FileInterface {
public:
    enum {
        O_READ,
        O_WRITE,
    };

    File();
    virtual ~File();

    bool open(const String& path, int mode = O_READ);
    void close();

public:
    virtual bool eof() const override;
    virtual void seek(int relative, size_t offset) override;
    virtual size_t tell() override;
    virtual bool readable() const override;
    virtual bool writable() const override;

protected:
    virtual Result<size_t> _read(void* buf, size_t n) override;
    virtual void _write(void* buf, size_t n) override;

private:
    FILE* fp_;
    int mode_;
};

class FileInterfaceFactory : private noncopyable {
public:
    FileInterfaceFactory() { }
    virtual ~FileInterfaceFactory() { }
    virtual Ptr<FileInterface> create_file(
        const String& path, int mode = File::O_READ)
        = 0;
};

class FileFactory : public FileInterfaceFactory {
public:
    FileFactory() { }
    virtual ~FileFactory() { }
    virtual Ptr<FileInterface> create_file(
        const String& path, int mode = File::O_READ) override;
};

} // namespace nup

#endif