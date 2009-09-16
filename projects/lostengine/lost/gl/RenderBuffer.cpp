#include "lost/gl/RenderBuffer.h"

#include "lost/lgl/lgl.h"
#include "lost/gl/Utils.h"

namespace lost
{
namespace gl
{

RenderBuffer::RenderBuffer()
{
  lglGenRenderbuffers(1, &buffer);GLDEBUG_THROW;
}

GLint RenderBuffer::width() { return param(LGL_RENDERBUFFER_WIDTH); }
GLint RenderBuffer::height() { return param(LGL_RENDERBUFFER_HEIGHT); }
GLint RenderBuffer::internalFormat() { return param(LGL_RENDERBUFFER_INTERNAL_FORMAT); }
GLint RenderBuffer::redSize() { return param(LGL_RENDERBUFFER_RED_SIZE); }
GLint RenderBuffer::greenSize() { return param(LGL_RENDERBUFFER_GREEN_SIZE); }
GLint RenderBuffer::blueSize() { return param(LGL_RENDERBUFFER_BLUE_SIZE); }
GLint RenderBuffer::alphaSize() { return param(LGL_RENDERBUFFER_ALPHA_SIZE); }
GLint RenderBuffer::depthSize() { return param(LGL_RENDERBUFFER_DEPTH_SIZE); }
GLint RenderBuffer::stencilSize() { return param(LGL_RENDERBUFFER_STENCIL_SIZE); }

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