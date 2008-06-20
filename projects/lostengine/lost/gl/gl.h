#ifndef LOST_GL_GL_H
#define LOST_GL_GL_H

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
