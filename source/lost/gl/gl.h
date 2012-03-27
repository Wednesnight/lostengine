/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/** This is the master GL header for the lost engine.
 * Include this file wherever you require GL functionality and need to access the API.
 * Depending on the platform, this will give you either plain gl/glu/glut calls or
 * their equivalent GL ES versions.
 * If you don't want to be bothered with the GL/ES differences, you can include the lgl/lglu headers
 * that provide a thin abstraction layer and common interfaces to some of the functions provided by gl/glu.
 */

#ifndef LOST_GL_GL_H
#define LOST_GL_GL_H

  #include "lost/platform/Type.h"

  // in case of iphone, this header includes the OpenGL ES interfaces
  // every other platform gets the standard gl headers plus helpers provided
  // by GLee (extension handling) and GLFW (platform abstraction)
  #if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE || defined ANDROID
    #define OPENGL_ES_1_1 11
    #define OPENGL_ES_2_0 20

    #if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
      #import <OpenGLES/ES2/gl.h>
      #import <OpenGLES/ES2/glext.h>
      #define VERSION_OPENGL_ES OPENGL_ES_1_1
    #else
      #include <EGL/egl.h>
      #include <GLES2/gl2.h>
      #include <GLES2/gl2ext.h>
      #define VERSION_OPENGL_ES OPENGL_ES_2_0
    #endif

    #define TARGET_OPENGL    0
    #define TARGET_OPENGL_ES 1
  #else
    #include "GLee.h"
    #if defined WIN32
      #include <GL/gl.h>
      #include <GL/glext.h>
      #include <GL/glu.h>
      #include <wingdi.h>
    #elif defined linux
      #include <GL/gl.h>
      #include <GL/glx.h>
      #include <GL/glext.h>
      #include <GL/glu.h>
    #elif defined __APPLE__
      #include <OpenGL/gl.h>
      #include <OpenGL/glext.h>
      #include <OpenGL/CGLTypes.h>
      #include <OpenGL/CGLCurrent.h>
      #include <OpenGL/glu.h>
    #endif

    #define TARGET_OPENGL    1
    #define TARGET_OPENGL_ES 0
  #endif
#endif
