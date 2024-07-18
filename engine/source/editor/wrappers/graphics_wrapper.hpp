//
// Created by Akira Mujawar on 06/07/22.
//

#ifndef PHYSICAT_GRAPHICS_WRAPPER_HPP
#define PHYSICAT_GRAPHICS_WRAPPER_HPP

#pragma once

#if defined(__EMSCRIPTEN__)
//    #include "GLES2/gl2.h"
    #include "GLES3/gl3.h"
    #define USING_GLES
#elif defined(__APPLE__)
    //Ignore the OpenGL deprecation warnings.
    #define GL_SILENCE_DEPRECATION
    #include "TargetConditionals.h"

    #if TARGET_OS_IPHONE
        #include <OpenGLES/ES2/gl.h>
        #define USING_GLES
    #else
        #include <OpenGL/gl3.h>
    #endif
#elif __ANDROID__
    #include "GLES2/gl2.h"
    #define USING_GLES
#elif WIN32
    #define GLEW_STATIC
    #include <GL/glew.h>
#endif


class graphics_wrapper {

};


#endif //PHYSICAT_GRAPHICS_WRAPPER_HPP
