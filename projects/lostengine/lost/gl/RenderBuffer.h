#ifndef LOST_GL_RENDERBUFFER_H
#define LOST_GL_RENDERBUFFER_H

#include "lost/lgl/lgl.h"
#include "lost/gl/Utils.h"

namespace lost
{
namespace gl
{
struct RenderBuffer
{
  RenderBuffer()
  {
    lglGenRenderbuffers(1, &buffer);GLDEBUG_THROW;
  }

  void enable()
  {
    lglBindRenderbuffer(LGL_RENDERBUFFER, buffer);GLDEBUG_THROW;
  }

  void disable()
  {
    lglBindRenderbuffer(LGL_RENDERBUFFER, 0);GLDEBUG_THROW;
  }

  void storage(GLenum inInternalFormat, GLsizei inWidth, GLsizei inHeight)
  {
    lglRenderbufferStorage(LGL_RENDERBUFFER, inInternalFormat, inWidth, inHeight);GLDEBUG_THROW;
  }

  GLint width() { return param(LGL_RENDERBUFFER_WIDTH); }
  GLint height() { return param(LGL_RENDERBUFFER_HEIGHT); }
  GLint internalFormat() { return param(LGL_RENDERBUFFER_INTERNAL_FORMAT); }
  GLint redSize() { return param(LGL_RENDERBUFFER_RED_SIZE); }
  GLint greenSize() { return param(LGL_RENDERBUFFER_GREEN_SIZE); }
  GLint blueSize() { return param(LGL_RENDERBUFFER_BLUE_SIZE); }
  GLint alphaSize() { return param(LGL_RENDERBUFFER_ALPHA_SIZE); }
  GLint depthSize() { return param(LGL_RENDERBUFFER_DEPTH_SIZE); }
  GLint stencilSize() { return param(LGL_RENDERBUFFER_STENCIL_SIZE); }

  GLint param(GLenum paramName)
  {
    GLint result;
    lglGetRenderbufferParameteriv(buffer, paramName, &result);GLDEBUG_THROW;
    return result;
  }

  virtual ~RenderBuffer()
  {
    lglDeleteRenderbuffers(1, &buffer);GLDEBUG;
  }

  GLuint  buffer;
};
}
}

#endif
