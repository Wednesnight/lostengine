#include "lost/gl/RenderBuffer.h"#ifndef LOST_GL_RENDERBUFFER_H

namespace lost
{
namespace gl
{

RenderBuffer::RenderBuffer()
{
  lglGenRenderbuffers(1, &buffer);GLDEBUG_THROW;
}

void RenderBuffer::enable()
{
  lglBindRenderbuffer(LGL_RENDERBUFFER, buffer);GLDEBUG_THROW;
}

void RenderBuffer::disable()
{
  lglBindRenderbuffer(LGL_RENDERBUFFER, 0);GLDEBUG_THROW;
}

void RenderBuffer::storage(GLenum inInternalFormat, GLsizei inWidth, GLsizei inHeight)
{
  lglRenderbufferStorage(LGL_RENDERBUFFER, inInternalFormat, inWidth, inHeight);GLDEBUG_THROW;
}

GLint RenderBuffer::param(GLenum paramName)
{
  GLint result;
  lglGetRenderbufferParameteriv(buffer, paramName, &result);GLDEBUG_THROW;
  return result;
}

RenderBuffer::~RenderBuffer()
{
  lglDeleteRenderbuffers(1, &buffer);GLDEBUG;
}

}
}