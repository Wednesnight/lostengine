#ifndef LOST_GL_FRAGMENTSHADER_H
#define LOST_GL_FRAGMENTSHADER_H

#include "lost/gl/Shader.h"

namespace lost
{
  namespace gl
  {
    struct FragmentShader;
    typedef lost::shared_ptr<FragmentShader> FragmentShaderPtr;

    struct FragmentShader : public Shader
    {
      FragmentShader();
      static ShaderPtr create() { return FragmentShaderPtr(new FragmentShader); }
    };
  }
}

#endif
