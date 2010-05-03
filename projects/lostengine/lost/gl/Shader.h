#ifndef LOST_GL_SHADER_H
#define LOST_GL_SHADER_H

#include "lost/gl/gltypes.h"
#include "lost/gl/forward.h"
#include <string>

namespace lost
{
namespace gl
{
struct Shader 
{
  Shader(GLenum type);
  virtual ~Shader();
  void source(const std::string& inSource);
  void compile();
  bool compiled();
  GLint param(GLenum paramName);
  std::string log();
  
  GLuint shader;
};
}
}

#endif
