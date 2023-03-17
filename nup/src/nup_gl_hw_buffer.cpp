#include "nup_gl_hw_buffer.h"

namespace nup {

static GLuint buffer_target_table[] = {
    // HardwareBufferType::VERTEX
    GL_ARRAY_BUFFER,
    // HardwareBufferType::INDICE
    GL_ELEMENT_ARRAY_BUFFER,
};

static GLuint usage_table[] = {
    GL_DYNAMIC_DRAW,
    GL_DYNAMIC_READ,
    GL_DYNAMIC_COPY,

    GL_STATIC_DRAW,
    GL_STATIC_READ,
    GL_STATIC_COPY,

    GL_STREAM_DRAW,
    GL_STREAM_READ,
    GL_STREAM_COPY,
};

#define gl_bind_target buffer_target_table[(int)buffer_type_]
#define gl_usage       usage_table[(int)usage_]

void HardwardBufferGL::resize(size_t size)
{
    // glBufferData();
    _bind();
    _resize(size);
}

void HardwardBufferGL::update_date(offset_t offset, void* ptr, size_t size)
{
    _bind();
    update_date(offset, ptr, size);
}

HardwardBufferGL::HardwardBufferGL(
    HardwareBufferType buffer_type, HardwareBufferUsage usage)
: HardwareBuffer(buffer_type, usage)
, native_id_(0)
{
    glGenBuffers(1, &native_id_);
}

HardwardBufferGL::~HardwardBufferGL()
{
    if (native_id_)
        glDeleteBuffers(1, &native_id_);
}

void HardwardBufferGL::_resize(size_t size)
{
    glBufferData(gl_bind_target, size, nullptr, gl_usage);
}

void HardwardBufferGL::_update_date(offset_t offset, void* ptr, size_t size)
{
    glBufferSubData(gl_bind_target, offset, size, ptr);
}

void HardwardBufferGL::_bind()
{
    auto& cur_bound_id = bound_native_ids_[(int)buffer_type_];
    if (native_id_ != cur_bound_id) {
        cur_bound_id = native_id_;
        glBindBuffer(gl_bind_target, native_id_);
    }
}

void HardwardBufferGL::bind() { _bind(); }

GLuint HardwardBufferGL::bound_native_ids_[(int)HardwareBufferType::COUNT]
    = { 0 };

// GLuint HardwardBufferGL::_bind_target() const {
//     return buffer_target_table[buffer_type_];
// }

} // namespace nup
// namespace nup
