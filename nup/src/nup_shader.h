#ifndef NUP_SHADER_H
#define NUP_SHADER_H
#include "type.h"

namespace nup {

class Shader : private boost::noncopyable {
public:
    Shader() { }
    virtual ~Shader() = 0;
    virtual void use() = 0;
};

class ShaderFactory : private boost::noncopyable {
public:
    ShaderFactory() { }
    virtual ~ShaderFactory() { }
    virtual Ptr<Shader> create_render_shader(const string& vs, const string& fs)
        = 0;
    virtual Ptr<Shader> create_compute_shader(const string& cs) = 0;
};

} // namespace nup
#endif