#ifndef LOST_LGL_LGL_GLES_H
#define LOST_LGL_LGL_GLES_H

#include "lost/gl/gl.h"

static inline void lglOrtho(GLfloat left,
              GLfloat right,
              GLfloat bottom,
              GLfloat top,
              GLfloat near,
              GLfloat far)
{
  glOrthof(left,
           right,
           bottom,
           top,
           near,
           far);
}


#endif
