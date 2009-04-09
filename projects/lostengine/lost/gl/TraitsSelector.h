#ifndef LOST_GL_TRAITSSELECTOR_H
#define LOST_GL_TRAITSSELECTOR_H

#include "lost/gl/gl.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"
#include "lost/model/Vertex.h"
#include "lost/common/Color.h"
#include <boost/cstdint.hpp>

namespace lost
{
  namespace gl
  {

    template<typename T>
    struct TraitsSelector
    {
      TraitsSelector()
      {
      }

    };

    template<>
    struct TraitsSelector<lost::math::Vec2>
    {
      GLint size;
      GLenum type;
      TraitsSelector() : size(2), type(GL_FLOAT) {}
    };

    template<>
    struct TraitsSelector<lost::math::Vec3>
    {
      GLint size;
      GLenum type;
      TraitsSelector() : size(3), type(GL_FLOAT) {}
    };

    template<>
    struct TraitsSelector<lost::math::Vec4>
    {
      GLint size;
      GLenum type;
      TraitsSelector() : size(4), type(GL_FLOAT) {}
    };

    template<>
    struct TraitsSelector<lost::common::Color>
    {
      GLint size;
      GLenum type;
      TraitsSelector() : size(4), type(GL_FLOAT) {}
    };

    template<>
    struct TraitsSelector<lost::model::Vertex>
    {
      GLint size;
      GLenum type;
      TraitsSelector() : size(3), type(GL_FLOAT) {}
    };
    
    template<>
    struct TraitsSelector<boost::uint8_t>
    {
      GLint size;
      GLenum type;
      TraitsSelector() : size(1), type(GL_UNSIGNED_BYTE) {}
    };

    template<>
    struct TraitsSelector<boost::uint16_t>
    {
      GLint size;
      GLenum type;
      TraitsSelector() : size(2), type(GL_UNSIGNED_SHORT) {}
    };

#if !TARGET_IPHONE_SIMULATOR && !TARGET_IPHONE
    template<>
    struct TraitsSelector<boost::uint32_t>
    {
      GLint size;
      GLenum type;
      TraitsSelector() : size(4), type(GL_UNSIGNED_INT) {}
    };
#endif
  
  }
}

#endif
