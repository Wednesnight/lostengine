#ifndef RAY_H
#define RAY_H

#include "lost/math/Vec3.h"

using namespace lost::math;

struct Ray {

  Vec3 origin;
  Vec3 direction;

  Ray(Vec3& origin, Vec3& direction)
  : origin(origin), direction(direction)
  {}

};

#endif
