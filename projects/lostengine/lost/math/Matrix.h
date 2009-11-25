#ifndef LOST_MATH_MATRIX_H
#define LOST_MATH_MATRIX_H

#include <algorithm>
#include "lost/math/Vec4.h"
#include "lost/math/Vec3.h"
#include <stdexcept>
#include "lost/math/lmath.h"
#include "lost/common/Logger.h"
#include "lost/math/Rect.h"
#include "lost/math/Vec2.h"

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
  std::ostream& operator<<(std::ostream& s, const lost::math::Matrix& m);
  
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
    // named constant to avoid magic number
    static const unsigned long numvals = 16;
    // holds all matrix values as required by OpenGL
    float m[numvals];

    // zero all elements
    void zero()
    {
      for(unsigned long i=0; i<numvals; ++i)
      {
        m[i] = 0.0f;
      }
    }

    // create identity matrix
    void initIdentity()
    {
      zero();
      m[0] = 1.0f;
      m[5] = 1.0f;
      m[10] = 1.0f;
      m[15] = 1.0f;
    }

    // initialises the matrix to be a translation matrix with the given values
    void initTranslation(const lost::math::Vec3& t)
    {
      initIdentity();
      m[12] = t.x;
      m[13] = t.y;
      m[14] = t.z;
    }

    // initialises the matrix to be a scaling matrix with the given values
    // each component of the vector denotes a scaling factor along the corresponding axis
    void initScaling(const lost::math::Vec3& s)
    {
      initIdentity();
      m[0] = s.x;
      m[5] = s.y;
      m[10] = s.z;
    }

    void initRotateX(float angleDeg)
    {
      initIdentity();
      float ar = lost::math::deg2rad(angleDeg);
      m[5] = cos(ar);m[9] = sin(ar);
      m[6] = -1.0f*sin(ar);m[10] = cos(ar);
    }

    void initRotateY(float angleDeg)
    {
      initIdentity();
      float ar = lost::math::deg2rad(angleDeg);
      m[0]=cos(ar); m[8]=-1.0f*sin(ar);
      m[2]=sin(ar); m[10]=cos(ar);
    }

    void initRotateZ(float angleDeg)
    {
      initIdentity();
      float ar = lost::math::deg2rad(angleDeg);
      m[0]=cos(ar); m[4]=sin(ar);
      m[1]=-1.0f*sin(ar); m[5]=cos(ar);
    }

    void initOrtho(const Rect& rect, const Vec2& nearAndFar)
    {
      zero();
      m[0] = 2 / (rect.width - rect.x);
      m[5] = 2 / (rect.height - rect.y);
      m[10] = -2 / (nearAndFar.far - nearAndFar.near);
      m[12] = -((rect.width + rect.x) / (rect.width - rect.x));
      m[13] = -((rect.height + rect.y) / (rect.height - rect.y));
      m[14] = -((nearAndFar.far + nearAndFar.near) / (nearAndFar.far - nearAndFar.near));
      m[15] = 1.0;
    }

    void initPerspective(const float& fovy, const float& aspect, const Vec2& nearAndFar)
    {
      float radFovY = lost::math::deg2rad(fovy / 2.0f);
      float f = cos(radFovY) / sin(radFovY);
      float deltaZ = nearAndFar.far - nearAndFar.near;
      
      float m0 = f / aspect;
      float m5 = f;
      float m10 = - (nearAndFar.far + nearAndFar.near) / deltaZ;
      float m11 = -1;
      float m14 = (-2 * nearAndFar.near * nearAndFar.far) / deltaZ;
      
      zero();
      m[0] = m0;
      m[5] = m5;
      m[10] = m10;
      m[11] = m11;
      m[14] = m14;
    }

    void initLookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
    {
      Vec3 upNormal = up;
      Vec3 f = center - eye;
      normalise(f);
      normalise(upNormal);
      Vec3 s = cross(f, upNormal);
      Vec3 u = cross(s, f);

      zero();

      m[0] = s.x;
      m[1] = u.x;
      m[2] = -1.0f*f.x;

      m[4] = s.y;
      m[5] = u.y;
      m[6] = -1.0f*f.y;

      m[8] = s.z;
      m[9] = u.z;
      m[10] = -1.0f*f.z;

      m[15] = 1;

      Matrix translation;
      translation.initTranslation(eye * -1.0f);
      *this = *this * translation;
    }

    void transpose()
    {
      std::swap(m[1], m[4]);
      std::swap(m[2], m[8]);
      std::swap(m[6], m[9]);std::swap(m[3], m[12]);
      std::swap(m[7], m[13]);
      std::swap(m[11], m[14]);
    }

    // mult vector

    // return the specified row as vec4
    lost::math::Vec4 row(long num) const
    {
      assert((num >= 0) && (num <=3));
      switch(num)
      {
        case 0:return lost::math::Vec4(m[0], m[4], m[8], m[12]);
        case 1:return lost::math::Vec4(m[1], m[5], m[9], m[13]);
        case 2:return lost::math::Vec4(m[2], m[6], m[10], m[14]);
        case 3:return lost::math::Vec4(m[3], m[7], m[11], m[15]);
        default: throw std::runtime_error("impossible");
      }
    }

    // return the specified column as vec4
    lost::math::Vec4 col(long num) const
    {
      assert((num >= 0) && (num <=3));
      switch(num)
      {
        case 0:return lost::math::Vec4(m[0], m[1], m[2], m[3]);
        case 1:return lost::math::Vec4(m[4], m[5], m[6], m[7]);
        case 2:return lost::math::Vec4(m[8], m[9], m[10], m[11]);
        case 3:return lost::math::Vec4(m[12], m[13], m[14], m[15]);
        default: throw std::runtime_error("impossible");
      }
    }
  };

  inline lost::math::Matrix operator*(const lost::math::Matrix& lhs, const lost::math::Matrix& rhs)
  {
    lost::math::Matrix result;

    lost::math::Vec4 tmp = rhs.col(0);
    result.m[0] = lhs.row(0) * tmp;
    result.m[1] = lhs.row(1) * tmp;
    result.m[2] = lhs.row(2) * tmp;
    result.m[3] = lhs.row(3) * tmp;

    tmp = rhs.col(1);
    result.m[4] = lhs.row(0) * tmp;
    result.m[5] = lhs.row(1) * tmp;
    result.m[6] = lhs.row(2) * tmp;
    result.m[7] = lhs.row(3) * tmp;

    tmp = rhs.col(2);
    result.m[8] = lhs.row(0) * tmp;
    result.m[9] = lhs.row(1) * tmp;
    result.m[10] = lhs.row(2) * tmp;
    result.m[11] = lhs.row(3) * tmp;

    tmp = rhs.col(3);
    result.m[12] = lhs.row(0) * tmp;
    result.m[13] = lhs.row(1) * tmp;
    result.m[14] = lhs.row(2) * tmp;
    result.m[15] = lhs.row(3) * tmp;

    return result;
  }

  inline lost::math::Vec3 operator*(const lost::math::Matrix& lhs, const lost::math::Vec3& rhs)
  {
    Vec4 result;
    Vec4 temp;

    temp.x = rhs.x;
    temp.y = rhs.y;
    temp.z = rhs.z;
    temp.w = 1.0f;

    result.x = lhs.row(0) * temp;
    result.y = lhs.row(1) * temp;
    result.z = lhs.row(2) * temp;
    result.w = lhs.row(3) * temp;

    return lost::math::Vec3(result.x, result.y, result.z);
  }

  inline lost::math::Vec4 operator*(const lost::math::Matrix& lhs, const lost::math::Vec4& rhs)
  {
    Vec4 result;

    result.x = lhs.row(0) * rhs;
    result.y = lhs.row(1) * rhs;
    result.z = lhs.row(2) * rhs;
    result.w = lhs.row(3) * rhs;

    return result;
  }

  inline bool operator==(const lost::math::Matrix& lhs, const lost::math::Matrix& rhs)
  {
    bool result = true;

    for(unsigned long i=0; i<Matrix::numvals; ++i)
    {
      if(lhs.m[i] != rhs.m[i])
      {
        result = false; break;
      }
    }

    return result;
  }

  inline bool operator!=(const lost::math::Matrix& lhs, const lost::math::Matrix& rhs)
  {
    return !(lhs == rhs);
  }


  inline std::ostream& operator<<(std::ostream& s, const lost::math::Matrix& m)
  {
    s << m.m[0] << " " << m.m[4] << " " << m.m[8] << " " << m.m[12] << std::endl;
    s << m.m[1] << " " << m.m[5] << " " << m.m[9] << " "  << m.m[13] << std::endl;
    s << m.m[2] << " " << m.m[6] << " " << m.m[10] << " " << m.m[14] << std::endl;
    s << m.m[3] << " " << m.m[7] << " " << m.m[11] << " " << m.m[15] << std::endl;
    return s;
  }
  
  struct MatrixRotX : public Matrix
  {
    MatrixRotX(float inAngle)
    {
      initRotateX(inAngle);
    }
  };
  
  struct MatrixRotY : public Matrix
  {
    MatrixRotY(float inAngle)
    {
      initRotateY(inAngle);
    }
  };
  
  struct MatrixRotZ : public Matrix
  {
    MatrixRotZ(float inAngle)
    {
      initRotateZ(inAngle);
    }
  };

  struct MatrixTranslation : public Matrix
  {
    MatrixTranslation(const lost::math::Vec3& inTranslation)
    {
      initTranslation(inTranslation);
    }
  };

  struct MatrixOrtho : public Matrix
  {
    MatrixOrtho(const Rect& rect, const Vec2& nearAndFar)
    {
      initOrtho(rect, nearAndFar);
    }
  };

  struct MatrixPerspective : public Matrix
  {
    MatrixPerspective(const float& fovy, const float& aspect, const Vec2& nearAndFar)
    {
      initPerspective(fovy, aspect, nearAndFar);
    }
  };
  
  struct MatrixLookAt : public Matrix
  {
    MatrixLookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
    {
      initLookAt(eye, center, up);
    }
  };
  
}
}

#endif