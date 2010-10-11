#ifndef LIGHT_H
#define LIGHT_H

#include "Sphere.h"

struct Light : public Sphere {
  
  Light(const Vec3& center, const float radius)
  : Sphere::Sphere(center, radius)
  {

    isLight = true;
  }

};

#endif
