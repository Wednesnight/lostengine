#ifndef LOST_GL_UNIFORMBLOCK_H
#define LOST_GL_UNIFORMBLOCK_H

#include "lost/gl/Variant.h"
#include <string>
#include <map>
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/gl/forward.h"

namespace lost
{
namespace gl
{
struct UniformBlock
{
  typedef std::map<std::string, Variant> VariantMap;
  void setFloat(const std::string& name, float v);
  void set(const std::string& name, const math::Vec2& v);
  void set(const std::string& name, const math::Vec3& v);
  void set(const std::string& name, const common::Color& v);
  
  static UniformBlockPtr create() { return UniformBlockPtr(new UniformBlock());}
  
  VariantMap variantMap;
};
}
}

#endif