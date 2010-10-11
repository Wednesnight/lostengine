#ifndef PLANE_H
#define PLANE_H

#include "Primitive.h"

#include "lost/math/Vec3.h"

using namespace lost::math;

struct Plane : public Primitive {

  Vec3 normal;
  float size;

  Plane(const Vec3& normal, const float size)
  : normal(normal), size(size)
  {}

  int intersect(Ray& ray, float& distance) {

    float d = normal * ray.direction;
    if (d != 0)
    {
      float dist = -(normal * ray.origin + size) / d;
      if (dist > 0)
      {
        if (dist < distance) 
        {
          distance = dist;
          return 1;
        }
      }
    }
    return 0;
  }
  
  Vec3 getLightNormal(const Vec3& intersection) {

    Vec3 result = intersection * -1.0f;
    return normalise(result);
  }

  Vec3 getLightVector(const Vec3& intersection) {
    
    return intersection * -1.0f;
  }
  
  Vec3 getNormal(const Vec3& intersection) {

    return normal;
  }

};

#endif
