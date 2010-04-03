#ifndef LOST_MATH_VEC4_H
#define LOST_MATH_VEC4_H

#include <iostream>
#include "lost/math/lmath.h"

namespace lost
{
  
  namespace math
  {
    
    struct Vec4
    {
      inline Vec4()
      : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
      {
      }
      
      inline Vec4(float inx, float iny, float inz)
      : x(inx), y(iny), z(inz), w(1.0f)
      {
      }
      
      inline Vec4(float inx, float iny, float inz, float inw)
      : x(inx), y(iny), z(inz), w(inw)
      {
      }
      
      void clear()
      {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 1.0f;    
      }
      
      float x;
      float y;
      float z;
      float w;
    };
    
    inline Vec4 operator*(const Vec4& inOp1,  float v)
    {
      Vec4 result;
      result.x = inOp1.x * v;
      result.y = inOp1.y * v;
      result.z = inOp1.z * v;
      result.w = inOp1.w * v;
      return result;
    }

    inline float operator*(const Vec4& inOp1, const Vec4& inOp2) 
    {
        return (inOp1.x*inOp2.x + inOp1.y*inOp2.y + inOp1.z*inOp2.z + inOp1.w*inOp2.w);
    }
    
    inline bool operator==(const Vec4& l, const Vec4& r)
    {
      if((l.x == r.x) && (l.y == r.y) && (l.z == r.z) && (l.w == r.w))
        return true;
      else
        return false;
    }
    
    inline Vec4 operator+(const Vec4& inOp1, const Vec4& inOp2)
    {
      Vec4 result;
      result.x = inOp1.x + inOp2.x;
      result.y = inOp1.y + inOp2.y;
      result.z = inOp1.z + inOp2.z;
      result.w = inOp1.w + inOp2.w;
      return result;
    }
    
    inline Vec4 operator-(const Vec4& inOp1, const Vec4& inOp2)
    {
      Vec4 result;
      result.x = inOp1.x - inOp2.x;
      result.y = inOp1.y - inOp2.y;
      result.z = inOp1.z - inOp2.z;
      result.w = inOp1.w - inOp2.w;
      return result;
    }
    
    // compares two vectors, taking epsilong into account for rounding errors
    inline bool compare(const Vec4& l, const Vec4& r, float epsilon)
    {
      if(   compare(l.x, r.x, epsilon)
         && compare(l.y, r.y, epsilon)
         && compare(l.z, r.z, epsilon)
         && compare(l.w, r.w, epsilon)
         )
        return true;
      else
        return false;
    }
    
    inline std::ostream& operator << (std::ostream& stream, const Vec4& v)
    {
      stream << "(" << v.x << "/" << v.y << "/" << v.z << "/" << v.w << ")";
      return stream;
    }
    
  } // math
  
} // lost

#endif
