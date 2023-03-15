#include "nup_opengl.h"

#if defined(NUP_USE_GL_4_3)
    #define GLAD_GL_IMPLEMENTATION
    #include "gl_4.3/gl.h"
#endif