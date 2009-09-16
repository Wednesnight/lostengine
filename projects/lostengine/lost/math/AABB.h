#ifndef LOST_MATH_AABB_H
#define LOST_MATH_AABB_H

#include "lost/math/Vec3.h"

namespace lost
{
  namespace math
  {
    
    struct AABB
    {
      Vec3 origin;
      Vec3 size;
      
      AABB(Vec3 inOrigin, Vec3 inSize)
      : origin(inOrigin),
        size(inSize)
      {
      }

      AABB()
      {
        AABB(Vec3(0,0,0), Vec3(0,0,0));
      }
    };
    
  }
}

#endif
