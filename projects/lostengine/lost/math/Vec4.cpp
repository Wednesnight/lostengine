#include "lost/math/Vec4.h"
#include "lost/math/lmath.h"

namespace lost
{  
namespace math
{
      Vec4::Vec4()
      : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
      {
      }
      
      Vec4::Vec4(float inx, float iny, float inz)
      : x(inx), y(iny), z(inz), w(1.0f)
      {
      }
      
      Vec4::Vec4(float inx, float iny, float inz, float inw)
      : x(inx), y(iny), z(inz), w(inw)
      {
      }
      
      void Vec4::clear()
      {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 1.0f;    
      }

     Vec4 operator*(const Vec4& inOp1,  float v)
    {
      Vec4 result;
      result.x = inOp1.x * v;
      result.y = inOp1.y * v;
      result.z = inOp1.z * v;
      result.w = inOp1.w * v;
      return result;
    }

     float operator*(const Vec4& inOp1, const Vec4& inOp2) 
    {
        return (inOp1.x*inOp2.x + inOp1.y*inOp2.y + inOp1.z*inOp2.z + inOp1.w*inOp2.w);
    }
    
     bool operator==(const Vec4& l, const Vec4& r)
    {
      if((l.x == r.x) && (l.y == r.y) && (l.z == r.z) && (l.w == r.w))
        return true;
      else
        return false;
    }
    
     Vec4 operator+(const Vec4& inOp1, const Vec4& inOp2)
    {
      Vec4 result;
      result.x = inOp1.x + inOp2.x;
      result.y = inOp1.y + inOp2.y;
      result.z = inOp1.z + inOp2.z;
      result.w = inOp1.w + inOp2.w;
      return result;
    }
    
     Vec4 operator-(const Vec4& inOp1, const Vec4& inOp2)
    {
      Vec4 result;
      result.x = inOp1.x - inOp2.x;
      result.y = inOp1.y - inOp2.y;
      result.z = inOp1.z - inOp2.z;
      result.w = inOp1.w - inOp2.w;
      return result;
    }
    
    // compares two vectors, taking epsilong into account for rounding errors
     bool compare(const Vec4& l, const Vec4& r, float epsilon)
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

}
}