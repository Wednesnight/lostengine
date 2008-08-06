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
      
      AABB()
      : origin(0.0f, 0.0f, 0.0f),
        size(0.0f, 0.0f, 0.0f)
      {
      }
    };
    
  }
}

#endif
