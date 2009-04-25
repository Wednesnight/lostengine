#ifndef LOST_MATH_QUAT_H
#define LOST_MATH_QUAT_H

#include "lost/math/Vec3.h"
#include "lost/math/Matrix.h"
#include "lost/math/lmath.h"

namespace lost
{
  namespace math
  {
    struct Quat;

    inline Quat operator *(const Quat& q1, const Quat& q2);
    inline Quat operator *(const Quat& q1, const Vec3& v2);
    inline Quat operator *(const Vec3& v1, const Quat& q2);
    inline Quat operator /(const Quat& q1, float f2);

    struct Quat
    {
      float x;
      float y;
      float z;
      float w;

      inline Quat()
      {
        zero();
      }

      inline Quat(const Vec3& vec)
      : x(vec.x), y(vec.y), z(vec.z), w(0.0f)
      {
      }

      inline Quat(const Vec3& axis, float angle)
      {
        initRotation(axis, angle);
      }
      
      inline Quat(float inx, float iny, float inz)
      : x(inx), y(iny), z(inz), w(0.0f)
      {
      }

      inline Quat(float inx, float iny, float inz, float inw)
      {
        initRotation(Vec3(inx,iny,inz), inw);
      }

      inline void zero()
      {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
      }

      inline float norm()
      {
        return sqrtf(x*x + y*y + z*z + w*w);
      }

      inline Quat scale(float s)
      {
        return Quat(x*s, y*s, z*s, w*s);
      }

      inline Quat conjugate()
      {
        return Quat(-x, -y, -z, w);
      }

      inline Quat inverse()
      {
        return conjugate() / norm();
      }

      /**
       * ensure that this is a unit quaternion
       */
      inline Quat normalize()
      {
        float m = norm();
        float nx = x/m;
        float ny = y/m;
        float nz = z/m;
        float nw = w/m;
        return Quat(nx,ny,nz,nw);
      }
      
      /**
       * axis has to be a unit vector
       */
      inline void initRotation(const Vec3& axis, float angle)
      {
        float t = deg2rad(angle)/2;
        float u = sinf(t);
        Vec3 nAxis = axis;
        normalise(nAxis);
        x = nAxis.x * u;
        y = nAxis.y * u;
        z = nAxis.z * u;
        w = cosf(t);
      }

      inline Vec3 rotate(const Vec3& vec)
      {
        Quat result = *this * vec * inverse();
        return Vec3(result.x,result.y,result.z);
      }

      inline Matrix matrix()
      {
        Matrix result;
        float xx = x*x;
        float xy = x*y;
        float xz = x*z;
        float xw = x*w;
        float yy = y*y;
        float yz = y*z;
        float yw = y*w;
        float zz = z*z;
        float zw = z*w;
        result.m[0]  = 1 - 2 * ( yy + zz );
        result.m[1]  =     2 * ( xy - zw );
        result.m[2]  =     2 * ( xz + yw );
        result.m[4]  =     2 * ( xy + zw );
        result.m[5]  = 1 - 2 * ( xx + zz );
        result.m[6]  =     2 * ( yz - xw );
        result.m[8]  =     2 * ( xz - yw );
        result.m[9]  =     2 * ( yz + xw );
        result.m[10] = 1 - 2 * ( xx + yy );
        result.m[3]  = result.m[7] = result.m[11] = result.m[12] = result.m[13] = result.m[14] = 0;
        result.m[15] = 1;
        return result;
      }

    };

    /**
     * q1*q2 is NOT equal to q2*q1
     * quaternion multiplication is not commutative
     */
    inline Quat operator *(const Quat& q1, const Quat& q2)
    {
      float x = (q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y);
      float y = (q1.w*q2.y + q1.y*q2.w + q1.z*q2.x - q1.x*q2.z);
      float z = (q1.w*q2.z + q1.z*q2.w + q1.x*q2.y - q1.y*q2.x);
      float w = (q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z);
      return Quat(x,y,z,w);
    }

    inline Quat operator *(const Quat& q1, const Vec3& v2)
    {
      return q1*Quat(v2);
    }
    
    inline Quat operator *(const Vec3& v1, const Quat& q2)
    {
      return Quat(v1)*q2;
    }

    inline Quat operator /(const Quat& q1, float f2)
    {
      return Quat(q1.x/f2, q1.y/f2, q1.z/f2, q1.w/f2);
    }

  }
}

#endif
