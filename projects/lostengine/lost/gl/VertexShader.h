#ifndef LOST_GL_VERTEXSHADER_H
#define LOST_GL_VERTEXSHADER_H

#include "lost/gl/Shader.h"

namespace lost
{
  namespace gl
  {
    struct VertexShader;
    typedef lost::shared_ptr<VertexShader> VertexShaderPtr;

    struct VertexShader : public Shader
    {
      VertexShader();
      static ShaderPtr create() { return VertexShaderPtr(new VertexShader); }
    };
  }
}

#endif
