#ifndef NUP_SHADER_H
#define NUP_SHADER_H
#include "type.h"

namespace nup {

class Shader : private noncopyable {
public:
    Shader() { }
    virtual ~Shader() = 0;
    virtual void use() = 0;
};

class ShaderFactory : private noncopyable {
public:
    ShaderFactory() { }
    virtual ~ShaderFactory() { }
    virtual Ptr<Shader> create_render_shader(const String& vs, const String& fs)
        = 0;
    virtual Ptr<Shader> create_compute_shader(const String& cs) = 0;
};

} // namespace nup
#endif