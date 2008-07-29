#ifndef LOST_GL_LGLU_H
#define LOST_GL_LGLU_H

namespace lost
{
namespace gl
{
  void lgluOrtho2D(GLdouble left,
              GLdouble right,
              GLdouble bottom,
              GLdouble top)
  {
    glOrtho(left, right, bottom, top, -1.0, 1-0);
  }
}
}


#endif