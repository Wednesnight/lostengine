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
