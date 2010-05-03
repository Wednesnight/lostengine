#ifndef LOST_GL_VERTEXSHADER_H
#define LOST_GL_VERTEXSHADER_H

#include "lost/gl/forward.h"
#include "lost/gl/Shader.h"

namespace lost
{
  namespace gl
  {
    struct VertexShader : public Shader
    {
      VertexShader();
      static ShaderPtr create() { return VertexShaderPtr(new VertexShader); }
    };
  }
}

#endif
