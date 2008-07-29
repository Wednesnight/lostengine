#ifndef LOST_LGL_LGL_GL_H
#define LOST_LGL_LGL_GL_H

#include "lost/gl/gl.h"

void lglOrtho(GLfloat left,
              GLfloat right,
              GLfloat bottom,
              GLfloat top,
              GLfloat near,
              GLfloat far)
{
  glOrtho(static_cast<GLdouble>(left),
          static_cast<GLdouble>(right),
          static_cast<GLdouble>(bottom),
          static_cast<GLdouble>(top),
          static_cast<GLdouble>(near),
          static_cast<GLdouble>(far));
}

#endif