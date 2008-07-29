/** This is the master GL header for the lost engine.
 * Include this file wherever you require GL functionality and need to access the API.
 * Depending on the platform, this will give you either plain gl/glu/glut calls or 
 * their equivalent GL ES versions.
 * If you don't want to be bothered with the GL/ES differences, you can include the lgl/lglu headers
 * that provide a thin abstraction layer and common interfaces to some of the functions provided by gl/glu.
 */

#ifndef LOST_GL_GL_H
#define LOST_GL_GL_H
  // TargetConditionals defines the IPHONE Macros we require
  // this header is only present if we build on an apple platform
  #if defined __APPLE__
  # include <TargetConditionals.h>
  #endif

  // in case of iphone, this header includes the OpenGL ES interfaces
  // every other platform gets the standard gl headers plus helpers provided 
  // by GLee (extension handling) and GLFW (platform abstraction)
  // FIXME: do we really need to include GLFW here?
  #if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_IPHONE)
    #import <OpenGLES/ES1/gl.h>
    #import <OpenGLES/ES1/glext.h>
  #else
    #include "GLee.h"
    #if defined WIN32
      #include <GL/gl.h>
      #include <GL/glext.h>
      #include <GLUT/glut.h>
    #elif defined linux
      #include <GL/gl.h>
      #include <GL/glext.h>
      #include <GL/glut.h>
    #elif defined __APPLE__
      #include <OpenGL/gl.h>
      #include <OpenGL/glext.h>
      #include <GLUT/glut.h>
    #endif
    #include "GL/glfw.h"
  #endif
#endif
