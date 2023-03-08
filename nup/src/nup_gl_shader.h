#ifndef NUP_GL_SHADER_H
#define NUP_GL_SHADER_H

#include "nup_shader.h"
#include "glad_gl.h"

namespace nup {

class GLShader : public Shader {
public:
    GLShader();
    virtual ~GLShader();

    GLuint native_id() const { return ID_; }

    virtual void use() override;

private:
    GLuint ID_;
};

class GLShaderFactory : public ShaderFactory {
public:
    GLShaderFactory() { }
    virtual ~GLShaderFactory() { }

    virtual Ptr<Shader> create_render_shader(
        const String& vs, const String& fs) override;
    virtual Ptr<Shader> create_compute_shader(const String& cs) override;
};

} // namespace nup
#endif