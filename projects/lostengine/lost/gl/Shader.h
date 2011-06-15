#ifndef LOST_GL_SHADER_H
#define LOST_GL_SHADER_H

#include "lost/gl/gltypes.h"
#include "lost/gl/forward.h"

namespace lost
{
namespace gl
{
struct Shader 
{
  Shader(GLenum type);
  virtual ~Shader();
  void source(const string& inSource);
  void compile();
  bool compiled();
  GLint param(GLenum paramName);
  string log();
  
  GLuint shader;
};
}
}

#endif
