#ifndef LOST_GL_UNIFORM_H
#define LOST_GL_UNIFORM_H

#include "lost/gl/gltypes.h"
#include "lost/common/Color.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"
#include "lost/math/Matrix.h"

namespace lost
{
namespace gl
{

struct Uniform
{
  string     name;
  GLint           index;
  GLenum          glType;     // numerical data type (int, float, bool etc.)
  GLint           size;       // size of array
  GLint           location;

  Uniform();
  Uniform(const string& inName,
            GLint inIndex,
            GLenum inGlType,
            GLint inSize,
            GLint loc);

  void operator=(float v);
  void operator=(const lost::common::Color& inCol);
  void operator=(const lost::math::Vec2& vec);
  void operator=(const lost::math::Vec3& vec);
  void operator=(const lost::math::Vec4& vec);
  void operator=(GLint v);

  void setInt(GLint inVal);
  void setFloat(float inVal);
  void setBool(bool inVal);
  void set(const lost::common::Color& inCol);
  void set(const lost::math::Vec4& vec);
  void set(const lost::math::Vec2& inVec);
  void set(const lost::math::Vec3& inVec);
  void set(const math::Matrix& mat);
};

}
}

#endif