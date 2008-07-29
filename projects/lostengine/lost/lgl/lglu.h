#ifndef LOST_LGL_LGLU_H
#define LOST_LGL_LGLU_H

#include "lost/lgl/lgl.h"

void lgluOrtho2D(GLfloat left,
            GLfloat right,
            GLfloat bottom,
            GLfloat top)
{
  lglOrtho(left, right, bottom, top, -1.0, 1.0);
}

#endif