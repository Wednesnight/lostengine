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

#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/gl/Shader.h"

namespace lost
{
namespace gl
{

  Shader::Shader(GLenum type)
  {
    shader = glCreateShader(type);GLDEBUG_THROW;
  }

  Shader::~Shader()
  {
    glDeleteShader(shader);
  }

  void Shader::source(const string& inSource)
  {
    const GLchar* sources[1];
    sources[0] = reinterpret_cast<const GLchar*>(inSource.c_str());
    GLint lengths[1];
    lengths[0] = inSource.size();
    glShaderSource(shader, 1, sources, lengths);GLDEBUG_THROW;
  }

  void Shader::compile()
  {
    glCompileShader(shader);GLDEBUG_THROW;
  }

  bool Shader::compiled()
  {
    return (param(GL_COMPILE_STATUS) == 0) ? false : true;
  }

  GLint Shader::param(GLenum paramName)
  {
    GLint result;
    glGetShaderiv(shader, paramName, &result);GLDEBUG_THROW;
    return result;
  }

  string Shader::log()
  {
    GLint size = param(GL_INFO_LOG_LENGTH);
    string result;
    if(size > 0)
    {
      shared_array<char> buffer(new char[size]);
      GLsizei length = 0;
      glGetShaderInfoLog(shader, size, &length, buffer.get());GLDEBUG_THROW;
      result = string(buffer.get(), buffer.get()+length);
    }
    return result;
  }



}
}
