#ifndef NUP_OPENGL_H
#define NUP_OPENGL_H

#include "nup_config.h"

#if defined(NUP_USE_GL_4_3)
    #include "gl_4.3/gl.h"
#endif

#if defined(NUP_USE_GLES2_3_2)
    #include "gles2_3.2/gles2.h"
#endif

#endif