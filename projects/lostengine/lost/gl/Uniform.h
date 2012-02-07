/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
