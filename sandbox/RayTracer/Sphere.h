#ifndef SPHERE_H
#define SPHERE_H

#include "Primitive.h"
#include "lost/math/lmath.h"

struct Sphere : public Primitive {

  Vec3 center;
  float radius;
  float sqRadius;
  float rRadius;

  Sphere(const Vec3& center, const float radius)
  : center(center), radius(radius), sqRadius(radius * radius), rRadius(1.0f / radius)
  {}

  int intersect(Ray& ray, float& distance) {
    
    Vec3 v = ray.origin - center;
    float b = (v * ray.direction) * -1.0f;
    float det = (b * b) - (v * v) + sqRadius;
    int retval = 0;
    if (det > 0)
    {
      det = sqrtf( det );
      float i1 = b - det;
      float i2 = b + det;
      if (i2 > 0)
      {
        if (i1 < 0) 
        {
          if (i2 < distance) 
          {
            distance = i2;
            retval = -1;
          }
        }
        else
        {
          if (i1 < distance)
          {
            distance = i1;
            retval = 1;
          }
        }
      }
    }
    return retval;
  }

  Vec3 getLightNormal(const Vec3& intersection) {

    Vec3 result = center - intersection;
    return normalise(result);
  }

  Vec3 getLightVector(const Vec3& intersection) {
    
    return center - intersection;
  }
  
  Vec3 getNormal(const Vec3& intersection) {

    return (intersection - center) * rRadius;
  }

};

#endif
