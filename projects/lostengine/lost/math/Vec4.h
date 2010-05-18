#ifndef LOST_MATH_VEC4_H
#define LOST_MATH_VEC4_H


namespace lost
{
  
  namespace math
  {
    
    struct Vec4
    {
      Vec4();      
      Vec4(float inx, float iny, float inz);      
      Vec4(float inx, float iny, float inz, float inw);      
      void clear();
      
      float x;
      float y;
      float z;
      float w;
    };
    
    Vec4 operator*(const Vec4& inOp1,  float v);
    float operator*(const Vec4& inOp1, const Vec4& inOp2);
    bool operator==(const Vec4& l, const Vec4& r);
    Vec4 operator+(const Vec4& inOp1, const Vec4& inOp2);
    Vec4 operator-(const Vec4& inOp1, const Vec4& inOp2);
    // compares two vectors, taking epsilong into account for rounding errors
    bool compare(const Vec4& l, const Vec4& r, float epsilon);
  } // math
} // lost

#endif
