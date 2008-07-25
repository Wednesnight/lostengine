#ifndef LOST_GL_TRAITSSELECTOR_H
#define LOST_GL_TRAITSSELECTOR_H

#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"

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

}
}

#endif