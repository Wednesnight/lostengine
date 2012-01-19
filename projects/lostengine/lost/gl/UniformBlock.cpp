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

#include "lost/gl/UniformBlock.h"

namespace lost
{
  namespace gl
  {

    void UniformBlock::setInt(const string& name, GLint v)
    {
      Variant variant;
      variant.i = v;
      variant.type = VT_int;
      variantMap[name] = variant;
    }
    
    void UniformBlock::setFloat(const string& name, float v)
    {
      Variant variant;
      variant.f = v;
      variant.type = VT_float;
      variantMap[name] = variant;
    }
    
    void UniformBlock::setBool(const string& name, bool v)
    {
      Variant variant;
      variant.b = v;
      variant.type = VT_bool;
      variantMap[name] = variant;
    }
    
    void UniformBlock::set(const string& name, const common::Color& v)
    {
      Variant variant;
      variant.color = v;
      variant.type = VT_color;
      variantMap[name] = variant;
    }
    
    void UniformBlock::set(const string& name, const math::Vec2& v)
    {
      Variant variant;
      variant.vec2 = v;
      variant.type = VT_vec2;
      variantMap[name] = variant;
    }

    void UniformBlock::set(const string& name, const math::Vec3& v)
    {
      Variant variant;
      variant.vec3 = v;
      variant.type = VT_vec3;
      variantMap[name] = variant;
    }

    void UniformBlock::set(const string& name, const math::Vec4& v)
    {
      Variant variant;
      variant.vec4 = v;
      variant.type = VT_vec4;
      variantMap[name] = variant;
    }
    
    void UniformBlock::set(const string& name, const math::Matrix& v)
    {
      Variant variant;
      variant.matrix = v;
      variant.type = VT_matrix;
      variantMap[name] = variant;
    }
    
  }
}
