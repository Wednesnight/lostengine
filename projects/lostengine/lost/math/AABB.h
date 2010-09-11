#ifndef LOST_MATH_AABB_H
#define LOST_MATH_AABB_H

#include "lost/math/Vec3.h"

namespace lost
{
  namespace math
  {
    
    struct AABB
    {
      Vec3 pos;
      Vec3 size;
      
      AABB(Vec3 inOrigin, Vec3 inSize);
      AABB();
    };
    
    bool intersect(const AABB& l, const AABB& r);
    
  }
}

#endif
