#ifndef NUP_HW_BUFFER_H
#define NUP_HW_BUFFER_H

#include "nup_type.h"
#include "nup_renderer_enum.h"

namespace nup {

class HardwareBuffer : private noncopyable {
public:
    HardwareBuffer(HardwareBufferType buffer_type, HardwareBufferUsage usage)
    : buffer_type_(buffer_type)
    , usage_(usage)
    {
    }
    virtual ~HardwareBuffer() { }
    virtual void resize(size_t size) = 0;
    virtual void update_date(offset_t offset, void* ptr, size_t size) = 0;
    virtual void bind() = 0;

protected:
    HardwareBufferType buffer_type_;
    HardwareBufferUsage usage_;
};

} // namespace nup

#endif