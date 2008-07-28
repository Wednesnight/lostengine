#ifndef LOST_MATH_VEC3_H
#define LOST_MATH_VEC3_H

#include "lost/math/lmath.h"

namespace lost
{

namespace math
{

struct Vec3
{
    union
    {
      struct {float x,y,z;};
      struct {float r,g,b;};
      struct {float v,vt,vn;};
    };



    Vec3() { zero();}
    Vec3(float inx, float iny, float inz) : x(inx), y(iny), z(inz) {}
    void zero() { x = y = z = 0.0f; }
};

inline float len(const Vec3& inOp)
{
    return std::sqrt(inOp.x*inOp.x + inOp.y*inOp.y + inOp.z*inOp.z);
}

inline float squlen(const Vec3& inOp)
{
    return (inOp.x*inOp.x + inOp.y*inOp.y + inOp.z*inOp.z);
}

inline void normalise(Vec3& ioOp)
{
    float l = len(ioOp);
    float x0 = 1.0f/l;

    ioOp.x *= x0;
    ioOp.y *= x0;
    ioOp.z *= x0;
}

inline Vec3 operator*(const Vec3& inOp1,  float v)
{
    Vec3 result;
    result.x = inOp1.x * v;
    result.y = inOp1.y * v;
    result.z = inOp1.z * v;
    return result;
}



inline float operator*(const Vec3& inOp1, const Vec3& inOp2)
{
    return (inOp1.x*inOp2.x + inOp1.y*inOp2.y + inOp1.z*inOp2.z);
}

inline Vec3 cross(const Vec3& inOp1, const Vec3& inOp2)
{
    return Vec3(inOp1.y * inOp2.z - inOp1.z * inOp2.y,
                inOp1.z * inOp2.x - inOp1.x * inOp2.z,
                inOp1.x * inOp2.y - inOp1.y * inOp2.x);
}

inline float angle(const Vec3& inOp1, const Vec3& inOp2)
{
  float result = 0.0f;
  float length = (len(inOp1) * len(inOp2));
  if (length > 0) result = rad2deg(acos((inOp1 * inOp2) / length));
  return result;
}
  
inline Vec3 operator+(const Vec3& inOp1, const Vec3& inOp2)
{
    Vec3 result;
    result.x = inOp1.x + inOp2.x;
    result.y = inOp1.y + inOp2.y;
    result.z = inOp1.z + inOp2.z;
    return result;
}

inline void operator +=(Vec3& inOp1, const Vec3& inOp2)
{
    inOp1.x += inOp2.x;
    inOp1.y += inOp2.y;
    inOp1.z += inOp2.z;
}

inline Vec3 operator-(const Vec3& inOp1, const Vec3& inOp2)
{
    Vec3 result;
    result.x = inOp1.x - inOp2.x;
    result.y = inOp1.y - inOp2.y;
    result.z = inOp1.z - inOp2.z;
    return result;
}

inline void operator -=(Vec3& inOp1, const Vec3& inOp2)
{
  inOp1.x -= inOp2.x;
  inOp1.y -= inOp2.y;
  inOp1.z -= inOp2.z;
}
  
  inline std::ostream& operator<<(std::ostream& s, const Vec3& v)
{
    s << v.x << " " << v.y << " "<< v.z;
    return s;
}

    // compares two vectors, taking epsilong into account for rounding errors
    inline bool compare(const Vec3& l, const Vec3& r, float epsilon)
    {
      if(   compare(l.x, r.x, epsilon)
         && compare(l.y, r.y, epsilon)
         && compare(l.z, r.z, epsilon)
         )
        return true;
      else
        return false;
    }

// reevaluate the following code later, not required right now
/*
// component wise multiplication, intended for rgb triples
inline Vec3 compmul(const Vec3& inOp1, const Vec3& inOp2)
{
    return Vec3(inOp1.x*inOp2.x, inOp1.y*inOp2.y, inOp1.z*inOp2.z);
}
*/

}

}

#endif
