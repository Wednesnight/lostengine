#ifndef LOST_MODEL_VERTEX_H
#define LOST_MODEL_VERTEX_H

#include "lost/math/Vec3.h"

namespace lost
{
  namespace model
  {

    struct Vertex : public math::Vec3
    {
    };

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
    
  }
}

#endif
