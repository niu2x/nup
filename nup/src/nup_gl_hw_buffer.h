#ifndef NUP_GL_HW_BUFFER_H
#define NUP_GL_HW_BUFFER_H

#include "nup_hw_buffer.h"
#include "nup_opengl.h"

namespace nup {

class HardwardBufferGL : public HardwareBuffer {
public:
    HardwardBufferGL(HardwareBufferType buffer_type, HardwareBufferUsage usage);
    virtual ~HardwardBufferGL();
    virtual void resize(size_t size) override;
    virtual void update_date(offset_t offset, void* ptr, size_t size) override;
    virtual void bind() override;

private:
    GLuint native_id_;

    void _resize(size_t size);
    void _update_date(offset_t offset, void* ptr, size_t size);
    void _bind();
    // GLuint _bind_target() const;

    static GLuint bound_native_ids_[(int)HardwareBufferType::COUNT];
};

} // namespace nup

#endif