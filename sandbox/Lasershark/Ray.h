#ifndef RAY_H
#define RAY_H

#include "lost/math/Vec3.h"

struct Ray
{
  lost::math::Vec3 pos;
  lost::math::Vec3 dir; // must be normalised
};

#endif