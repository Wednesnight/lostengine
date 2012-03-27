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

#ifndef LOST_MATH_MATRIX_H
#define LOST_MATH_MATRIX_H

#include "lost/math/forward.h"

namespace lost
{
  namespace math
  {
    struct Matrix;
    lost::math::Matrix operator*(const lost::math::Matrix& lhs, const lost::math::Matrix& rhs);
    lost::math::Vec3 operator*(const lost::math::Matrix& lhs, const lost::math::Vec3& rhs);
    lost::math::Vec4 operator*(const lost::math::Matrix& lhs, const lost::math::Vec4& rhs);
    bool operator==(const lost::math::Matrix& lhs, const lost::math::Matrix& rhs);
    bool operator!=(const lost::math::Matrix& lhs, const lost::math::Matrix& rhs);
    
    /* OpenGL compatible 4x4 matrix in column major order
     * From the blue book:
     *
     * m[0] m[4] m[8]  m[12]
     * m[1] m[5] m[9]  m[13]
     * m[2] m[6] m[10] m[14]
     * m[3] m[7] m[11] m[15]
     */
    struct Matrix
    {
      static const unsigned long numvals = 16; // named constant to avoid magic number
      float m[numvals]; // holds all matrix values as required by OpenGL

      void zero(); // zero all elements      
      void initIdentity(); // create identity matrix      
      void initTranslation(const lost::math::Vec3& t); // initialises the matrix to be a translation matrix with the given values

      // initialises the matrix to be a scaling matrix with the given values
      // each component of the vector denotes a scaling factor along the corresponding axis
      void initScaling(const lost::math::Vec3& s);
      void initRotateX(float angleDeg);
      void initRotateY(float angleDeg);
      void initRotateZ(float angleDeg);
      void initOrtho(const Rect& rect, const Vec2& nearAndFar);
      void initPerspective(const float& fovy, const float& aspect, const Vec2& nearAndFar);
      void initLookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
      void transpose();

      
      lost::math::Vec4 row(long num) const; // return the specified row as vec4
      void row(long num, const Vec4& row); // set the specified row as vec4
      lost::math::Vec4 col(long num) const; // return the specified column as vec4
    };

    lost::math::Matrix operator*(const lost::math::Matrix& lhs, const lost::math::Matrix& rhs);
    lost::math::Vec3 operator*(const lost::math::Matrix& lhs, const lost::math::Vec3& rhs);
    lost::math::Vec4 operator*(const lost::math::Matrix& lhs, const lost::math::Vec4& rhs);

    bool operator==(const lost::math::Matrix& lhs, const lost::math::Matrix& rhs);
    bool operator!=(const lost::math::Matrix& lhs, const lost::math::Matrix& rhs);


    
    struct MatrixRotation : public Matrix
    {
      MatrixRotation(const Vec3& inAngles);
    };

    struct MatrixRotX : public Matrix
    {
      MatrixRotX(float inAngle);
    };
    
    struct MatrixRotY : public Matrix
    {
      MatrixRotY(float inAngle);
    };
    
    struct MatrixRotZ : public Matrix
    {
      MatrixRotZ(float inAngle);
    };

    struct MatrixTranslation : public Matrix
    {
      MatrixTranslation(const lost::math::Vec3& inTranslation);
    };

    struct MatrixOrtho : public Matrix
    {
      MatrixOrtho(const Rect& rect, const Vec2& nearAndFar);
    };

    struct MatrixPerspective : public Matrix
    {
      MatrixPerspective(const float& fovy, const float& aspect, const Vec2& nearAndFar);
    };
    
    struct MatrixLookAt : public Matrix
    {
      MatrixLookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
    };

    struct MatrixScaling : public Matrix
    {
      MatrixScaling(const Vec3& scaling);
    };

  }
}

#endif
