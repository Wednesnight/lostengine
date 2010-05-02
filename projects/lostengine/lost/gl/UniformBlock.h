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
      void setInt(const std::string& inName, GLint inVal);
      void setFloat(const std::string& name, float v);
      void setBool(const std::string& name, bool v);
      void set(const std::string& name, const common::Color& v);
      void set(const std::string& name, const math::Vec2& v);
      void set(const std::string& name, const math::Vec3& v);
      void set(const std::string& name, const math::Vec4& v);
      void set(const std::string& inName, const math::Matrix& inVal);
      
      static UniformBlockPtr create() { return UniformBlockPtr(new UniformBlock());}
      
      VariantMap variantMap;
    };
  }
}

#endif
