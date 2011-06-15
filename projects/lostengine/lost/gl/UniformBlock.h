#ifndef LOST_GL_UNIFORMBLOCK_H
#define LOST_GL_UNIFORMBLOCK_H

#include "lost/gl/Variant.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/gl/forward.h"

namespace lost
{
  namespace gl
  {
    struct UniformBlock
    {
      typedef map<string, Variant> VariantMap;
      void setInt(const string& inName, GLint inVal);
      void setFloat(const string& name, float v);
      void setBool(const string& name, bool v);
      void set(const string& name, const common::Color& v);
      void set(const string& name, const math::Vec2& v);
      void set(const string& name, const math::Vec3& v);
      void set(const string& name, const math::Vec4& v);
      void set(const string& inName, const math::Matrix& inVal);
      
      static UniformBlockPtr create() { return UniformBlockPtr(new UniformBlock());}
      
      VariantMap variantMap;
    };
  }
}

#endif
