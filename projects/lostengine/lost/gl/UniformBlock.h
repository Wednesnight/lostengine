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
  void setFloat(const std::string& name, float v);
  void setVec2(const std::string& name, const math::Vec2& v);
  void setVec3(const std::string& name, const math::Vec3& v);
  
  static UniformBlockPtr create() { return UniformBlockPtr(new UniformBlock());}
  
  std::map<std::string, Variant> variantMap;
};
}
}

#endif