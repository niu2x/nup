#include "nup_opengl.h"

#if defined(NUP_USE_GL_4_3)
    #define GLAD_GL_IMPLEMENTATION
    #include "gl_4.3/gl.h"
#endif

#if defined(NUP_USE_GLES2_3_2)
    #define GLAD_GLES2_IMPLEMENTATION
    #include "gles2_3.2/gles2.h"
#endif
