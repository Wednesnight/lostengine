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

  void Shader::source(const std::string& inSource)
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

  std::string Shader::log()
  {
    GLint size = param(GL_INFO_LOG_LENGTH);
    std::string result;
    if(size > 0)
    {
      shared_array<char> buffer(new char[size]);
      GLsizei length = 0;
      glGetShaderInfoLog(shader, size, &length, buffer.get());GLDEBUG_THROW;
      result = std::string(buffer.get(), buffer.get()+length);
    }
    return result;
  }



}
}