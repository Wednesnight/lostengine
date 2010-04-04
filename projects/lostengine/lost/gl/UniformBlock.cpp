#include "lost/gl/UniformBlock.h"

namespace lost
{
namespace gl
{

void UniformBlock::setFloat(const std::string& name, float v)
{
  Variant variant;
  variant.f = v;
  variant.type = VT_float;
  variantMap[name] = variant;
}

void UniformBlock::set(const std::string& name, const math::Vec2& v)
{
  Variant variant;
  variant.vec2 = v;
  variant.type = VT_vec2;
  variantMap[name] = variant;
}

void UniformBlock::set(const std::string& name, const math::Vec3& v)
{
  Variant variant;
  variant.vec3 = v;
  variant.type = VT_vec3;
  variantMap[name] = variant;
}

void UniformBlock::set(const std::string& name, const common::Color& v)
{
  Variant variant;
  variant.color = v;
  variant.type = VT_color;
  variantMap[name] = variant;
}


}
}
