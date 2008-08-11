#ifndef LOST_GL_TRAITSSELECTOR_H
#define LOST_GL_TRAITSSELECTOR_H

#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"
#include <stdint.h>

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
    struct TraitsSelector<uint8_t>
    {
      GLint size;
      GLenum type;
      TraitsSelector() : size(1), type(GL_UNSIGNED_BYTE) {}
    };

    template<>
    struct TraitsSelector<uint16_t>
    {
      GLint size;
      GLenum type;
      TraitsSelector() : size(2), type(GL_UNSIGNED_SHORT) {}
    };

#if !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_IPHONE)
    template<>
    struct TraitsSelector<uint32_t>
    {
      GLint size;
      GLenum type;
      TraitsSelector() : size(4), type(GL_UNSIGNED_INT) {}
    };
#endif
  
  }
}

#endif
