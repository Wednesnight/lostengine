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

#ifndef LOST_MATH_QUAT_H
#define LOST_MATH_QUAT_H

#include "lost/math/forward.h"

namespace lost
{
  namespace math
  {
    struct Quat;

    Quat operator *(const Quat& q1, const Quat& q2);
    Quat operator *(const Quat& q1, const Vec3& v2);
    Quat operator *(const Vec3& v1, const Quat& q2);
    Quat operator /(const Quat& q1, float f2);

    struct Quat
    {
      float x;
      float y;
      float z;
      float w;

      Quat();
      Quat(const Vec3& vec);
      Quat(const Vec3& axis, float angle);
      Quat(float inx, float iny, float inz);
      Quat(float inx, float iny, float inz, float inw);
      void zero();
      float norm();
      Quat scale(float s);
      Quat conjugate();
      Quat inverse();

      /**
       * ensure that this is a unit quaternion
       */
      Quat normalize();

      
      /**
       * axis has to be a unit vector
       */
      void initRotation(const Vec3& axis, float angle);
      Vec3 rotate(const Vec3& vec);
      Matrix matrix();
    };

    /**
     * q1*q2 is NOT equal to q2*q1
     * quaternion multiplication is not commutative
     */
    Quat operator *(const Quat& q1, const Quat& q2);
    Quat operator *(const Quat& q1, const Vec3& v2);
    Quat operator *(const Vec3& v1, const Quat& q2);
    Quat operator /(const Quat& q1, float f2);
  }
}

#endif
