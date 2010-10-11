#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Ray.h"
#include "Material.h"

struct Primitive {

  Material material;
  bool isLight;

  Primitive()
  : isLight(false)
  {}

  // 0 = miss, 1 = hit, -1 = hit from inside primitive
  virtual int intersect(Ray& ray, float& distance) = 0;

  virtual Vec3 getLightNormal(const Vec3& intersection) = 0;
  virtual Vec3 getLightVector(const Vec3& intersection) = 0;
  virtual Vec3 getNormal(const Vec3& intersection) = 0;

};

#endif
