#ifndef LOST_GL_VARIANT_H
#define LOST_GL_VARIANT_H

#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"
#include "lost/math/Matrix.h"
#include "lost/gl/VariantType.h"

namespace lost
{
namespace gl
{

struct Variant
{
  // can't use union when members have constructors
  float   f;
  math::Vec2    vec2;
  math::Vec3    vec3;
  math::Vec4    vec4;
  math::Matrix  matrix;
  
  VariantType type;
};
}
}

#endif