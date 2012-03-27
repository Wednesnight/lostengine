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

#ifndef LOST_LGL_LGLU_H
#define LOST_LGL_LGLU_H

#include "lost/lgl/lgl.h"
#include <math.h>
#include "lost/math/Matrix.h"
#include "lost/math/Vec3.h"

/*static inline void lgluOrtho2D(GLfloat left,
            GLfloat right,
            GLfloat bottom,
            GLfloat top)
{
  lglOrtho(left, right, bottom, top, -1.0, 1.0);
}*/

/*// reimplementation of gluPerspective
// following the OpenGL blue book, version 1.4, p. 652
static inline void lgluPerspective(GLfloat fovy,
                     GLfloat aspect,
                     GLfloat zNear,
                     GLfloat zFar)
{
  float radFovY = lost::math::deg2rad(fovy / 2.0f);
  float f = cos(radFovY) / sin(radFovY);
  float deltaZ = zFar - zNear;

  float m0 = f / aspect;
  float m5 = f;
  float m10 = - (zFar + zNear) / deltaZ;
  float m11 = -1;
  float m14 = (-2 * zNear * zFar) / deltaZ;

  lost::math::Matrix matrix;
  matrix.zero();

  matrix.m[0] = m0;
  matrix.m[5] = m5;
  matrix.m[10] = m10;
  matrix.m[11] = m11;
  matrix.m[14] = m14;

  glMultMatrixf(matrix.m);
}*/

/*// reimplementation of gluLookAt
// following the OpenGL blue book, version 1.4, p. 627
static inline void lgluLookAt(GLfloat eyeX,
                GLfloat eyeY,
                GLfloat eyeZ,
                GLfloat centerX,
                GLfloat centerY,
                GLfloat centerZ,
                GLfloat upX,
                GLfloat upY,
                GLfloat upZ)
{
  lost::math::Vec3 eye(eyeX, eyeY, eyeZ);
  lost::math::Vec3 center(centerX, centerY, centerZ);
  lost::math::Vec3 up(upX, upY, upZ);

  lost::math::Vec3 f = center - eye;
  lost::math::normalise(f);
  lost::math::normalise(up);
  lost::math::Vec3 s = cross(f, up);
  lost::math::Vec3 u = cross(s, f);

  lost::math::Matrix matrix;
  matrix.zero();

  matrix.m[0] = s.x;
  matrix.m[1] = u.x;
  matrix.m[2] = -1.0f*f.x;

  matrix.m[4] = s.y;
  matrix.m[5] = u.y;
  matrix.m[6] = -1.0f*f.y;

  matrix.m[8] = s.z;
  matrix.m[9] = u.z;
  matrix.m[10] = -1.0f*f.z;

  matrix.m[15] = 1;

  glMultMatrixf(matrix.m);
  glTranslatef(-1.0f*eyeX, -1.0f*eyeY, -1.0f*eyeZ);
}*/

#endif
