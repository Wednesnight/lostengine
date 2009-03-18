#ifndef LOST_MATH_VEC2_H
#define LOST_MATH_VEC2_H

#include "lost/math/lmath.h"
#include <iostream>

namespace lost
{
  namespace math
  {

    struct Vec2
    {
        union
        {
            struct {float x,y;};
            struct {float width,height;};
            struct {float min,max;};
        };

        Vec2(float a, float b) : x(a), y(b) {}
        Vec2() { zero();}
        void zero() { x = y = 0.0f; }
    };

    inline float len(const Vec2& inOp)
    {
      return sqrt(inOp.x*inOp.x + inOp.y*inOp.y);
    }

    inline float squlen(const Vec2& inOp)
    {
      return (inOp.x*inOp.x + inOp.y*inOp.y);
    }

    inline Vec2& normalise(Vec2& ioOp)
    {
      float l = len(ioOp);
      float x0 = 1.0f/l;

      ioOp.x *= x0;
      ioOp.y *= x0;
      return ioOp;
    }

    inline Vec2 perpendicular(Vec2& v)
    {
      Vec2 result;

      result.x = -v.y;
      result.y = v.x;

      return result;
    }

    inline Vec2 operator*(const Vec2& inOp1,  float v)
    {
      Vec2 result;
      result.x = inOp1.x * v;
      result.y = inOp1.y * v;
      return result;
    }

    inline float operator*(const Vec2& inOp1, const Vec2& inOp2)
    {
      return (inOp1.x*inOp2.x + inOp1.y*inOp2.y);
    }
    
    inline Vec2 operator+(const Vec2& inOp1, const Vec2& inOp2)
    {
      Vec2 result;
      result.x = inOp1.x + inOp2.x;
      result.y = inOp1.y + inOp2.y;
      return result;
    }

    inline void operator +=(Vec2& inOp1, const Vec2& inOp2)
    {
      inOp1.x += inOp2.x;
      inOp1.y += inOp2.y;
    }

    inline Vec2 operator-(const Vec2& inOp1, const Vec2& inOp2)
    {
      Vec2 result;
      result.x = inOp1.x - inOp2.x;
      result.y = inOp1.y - inOp2.y;
      return result;
    }

    inline bool operator==(const Vec2& inOp1, const Vec2& inOp2)
    {
      return (inOp1.x == inOp2.x && inOp1.y == inOp2.y);
    }

    inline std::ostream& operator<<(std::ostream& stream, const lost::math::Vec2& vec)
    {
      stream << vec.x << " " << vec.y;
      return stream;
    }

    inline float angle(const Vec2& inOp1, const Vec2& inOp2)
    {
      float result = 0.0f;
      float length = (len(inOp1) * len(inOp2));
      if (length > 0) result = rad2deg(acos((inOp1 * inOp2) / length));
      return result;
    }
    
    // compares two vectors, taking epsilon into account for rounding errors
    inline bool compare(const Vec2& l, const Vec2& r, float epsilon)
    {
      if(   compare(l.x, r.x, epsilon)
         && compare(l.y, r.y, epsilon)
         )
        return true;
      else
        return false;
    }

  }
}

#endif
