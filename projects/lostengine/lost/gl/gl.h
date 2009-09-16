/** This is the master GL header for the lost engine.
 * Include this file wherever you require GL functionality and need to access the API.
 * Depending on the platform, this will give you either plain gl/glu/glut calls or 
 * their equivalent GL ES versions.
 * If you don't want to be bothered with the GL/ES differences, you can include the lgl/lglu headers
 * that provide a thin abstraction layer and common interfaces to some of the functions provided by gl/glu.
 */

#ifndef LOST_GL_GL_H
#define LOST_GL_GL_H

  #include "lost/platform/Platform.h"

  // in case of iphone, this header includes the OpenGL ES interfaces
  // every other platform gets the standard gl headers plus helpers provided 
  // by GLee (extension handling) and GLFW (platform abstraction)
  #if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
    #import <OpenGLES/ES1/gl.h>
    #import <OpenGLES/ES1/glext.h>

    #define OPENGL_ES_1_1 11
    #define OPENGL_ES_2_0 20

    #define TARGET_OPENGL    0
    #define TARGET_OPENGL_ES 1

    #define VERSION_OPENGL_ES OPENGL_ES_1_1
  #else
    #include "GLee.h"
    #if defined WIN32
      #include <GL/gl.h>
      #include <GL/glext.h>
      #include <GL/glu.h>
      #include <wingdi.h>
    #elif defined linux
      #include <GL/gl.h>
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
