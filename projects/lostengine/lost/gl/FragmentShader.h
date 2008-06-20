#ifndef LOST_GL_FRAGMENTSHADER_H
#define LOST_GL_FRAGMENTSHADER_H

#include "lost/gl/Shader.h"

namespace lost
{
namespace gl
{
struct FragmentShader : public Shader
{
  FragmentShader() : Shader(GL_FRAGMENT_SHADER) {}
};
}
}

#endif
