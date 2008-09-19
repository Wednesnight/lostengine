#ifndef LOST_MODEL_VERTEX_H
#define LOST_MODEL_VERTEX_H

#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"
#include "lost/math/Matrix.h"

namespace lost
{
  namespace model
  {

    struct Vertex : public math::Vec3
    {
      Vertex()
      : Vec3()
      {
      }

      Vertex(const float& inX, const float& inY, const float& inZ)
      : Vec3(inX, inY, inZ)
      {
      }
    };

    inline Vertex operator+(const Vertex& inOp1, const Vertex& inOp2)
    {
      Vertex result;
      result.x = inOp1.x + inOp2.x;
      result.y = inOp1.y + inOp2.y;
      result.z = inOp1.z + inOp2.z;
      return result;
    }
    
    inline void operator +=(math::Vec3& inOp1, const Vertex& inOp2)
    {
      inOp1.x += inOp2.x;
      inOp1.y += inOp2.y;
      inOp1.z += inOp2.z;
    }

    inline bool operator ==(const Vertex& inOp1, const Vertex& inOp2)
    {
      return (inOp1.x == inOp2.x) && (inOp1.y == inOp2.y) && (inOp1.z == inOp2.z);
    }

    inline Vertex operator*(const lost::math::Matrix& lhs, const Vertex& rhs)
    {
      Vertex           result;
      lost::math::Vec4 temp;
      
      temp.x = rhs.x;
      temp.y = rhs.y;
      temp.z = rhs.z;
      temp.w = 1.0f;
      
      result.x = lhs.row(0) * temp;
      result.y = lhs.row(1) * temp;
      result.z = lhs.row(2) * temp;
      
      return result;
    }

  }
}

#endif
