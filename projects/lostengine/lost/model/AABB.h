#ifndef LOST_MODEL_AABB_H
#define LOST_MODEL_AABB_H

#include "lost/math/Vec3.h"

namespace lost
{
  namespace model
  {
    
    struct AABB
    {
      lost::math::Vec3 origin;
      lost::math::Vec3 size;
      
      AABB()
      : origin(0.0f, 0.0f, 0.0f),
        size(0.0f, 0.0f, 0.0f)
      {
      }
    };
    
  }
}

#endif
