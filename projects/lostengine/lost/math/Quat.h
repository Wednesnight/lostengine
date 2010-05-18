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
