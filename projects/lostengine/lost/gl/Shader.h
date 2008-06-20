#ifndef LOST_GL_SHADER_H
#define LOST_GL_SHADER_H

#include "lost/gl/gl.h"
#include <string>
#include <boost/scoped_ptr.hpp>

namespace lost
{
namespace gl
{
struct Shader 
{
  Shader(GLenum type)
  {
    shader = glCreateShader(type);GLDEBUG_THROW;
  }

  void source(const std::string& inSource)
  {
    const GLchar* sources[1];
    sources[0] = reinterpret_cast<const GLchar*>(inSource.c_str());
    GLint lengths[1];
    lengths[0] = inSource.size();
    glShaderSource(shader, 1, sources, lengths);GLDEBUG_THROW;
  }

  void compile()
  {
    glCompileShader(shader);GLDEBUG_THROW;
  }

  bool compiled()
  {
    return (param(GL_COMPILE_STATUS) == 0) ? false : true;
  }

  GLint param(GLenum paramName)
  {
    GLint result;
    glGetShaderiv(shader, paramName, &result);GLDEBUG_THROW;
    return result;
  }

  std::string log()
  {
    GLint size = param(GL_INFO_LOG_LENGTH);
    boost::scoped_array<char> buffer;
    std::string result;
    if(size > 0)
    {
      boost::scoped_array<char> buffer(new char[size]);
      GLsizei length = 0;
      glGetShaderInfoLog(shader, size, &length, buffer.get());GLDEBUG_THROW;
      result = std::string(buffer.get(), buffer.get()+length);
    }
    return result;
  }

  virtual ~Shader()
  {
    glDeleteShader(shader);
  }
  
  GLuint shader;
};
}
}

#endif
