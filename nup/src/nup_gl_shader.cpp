#include "nup_gl_shader.h"
#include <iostream>

static void checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout
                << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                << infoLog
                << "\n -- "
                   "--------------------------------------------------- -- "
                << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout
                << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                << infoLog
                << "\n -- "
                   "--------------------------------------------------- -- "
                << std::endl;
        }
    }
}

namespace nup {
GLShader::GLShader() { ID_ = glCreateProgram(); }
GLShader::~GLShader() { glDeleteProgram(ID_); }

Ptr<Shader> GLShaderFactory::create_render_shader(
    const string& vs, const string& fs)
{

    auto shader = NUP_MAKE_PTR(GLShader);

    const char* vc = vs.c_str();
    const char* fc = fs.c_str();

    auto v_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_id, 1, &vc, NULL);
    glCompileShader(v_id);
    checkCompileErrors(v_id, "VERTEX");
    NUP_CHECK_GL_ERROR();

    auto f_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_id, 1, &fc, NULL);
    glCompileShader(f_id);
    checkCompileErrors(f_id, "FRAGMENT");
    NUP_CHECK_GL_ERROR();

    // shader Program
    glAttachShader(shader->native_id(), v_id);
    glAttachShader(shader->native_id(), f_id);
    glLinkProgram(shader->native_id());
    checkCompileErrors(shader->native_id(), "PROGRAM");
    NUP_CHECK_GL_ERROR();

    return shader;
}

Ptr<Shader> GLShaderFactory::create_compute_shader(const string& cs)
{

    auto shader = NUP_MAKE_PTR(GLShader);
    const char* cShaderCode = cs.c_str();

    auto compute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute, 1, &cShaderCode, NULL);
    glCompileShader(compute);
    checkCompileErrors(compute, "COMPUTE");
    NUP_CHECK_GL_ERROR();

    // shader Program
    glAttachShader(shader->native_id(), compute);
    glLinkProgram(shader->native_id());
    checkCompileErrors(shader->native_id(), "PROGRAM");
    NUP_CHECK_GL_ERROR();

    return shader;
}

void GLShader::use() { glUseProgram(ID_); }

} // namespace nup