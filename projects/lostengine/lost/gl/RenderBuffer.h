#ifndef LOST_GL_RENDERBUFFER_H
#define LOST_GL_RENDERBUFFER_H

#include "lost/lgl/lgl.h"
#include "lost/gl/Utils.h"

namespace lost
{
namespace gl
{
struct RenderBuffer;
typedef boost::shared_ptr<RenderBuffer> RenderBufferPtr;

struct RenderBuffer
{
  RenderBuffer();
  virtual ~RenderBuffer();

  void enable();
  void disable();
  void storage(GLenum inInternalFormat, GLsizei inWidth, GLsizei inHeight);

  GLint width() { return param(LGL_RENDERBUFFER_WIDTH); }
  GLint height() { return param(LGL_RENDERBUFFER_HEIGHT); }
  GLint internalFormat() { return param(LGL_RENDERBUFFER_INTERNAL_FORMAT); }
  GLint redSize() { return param(LGL_RENDERBUFFER_RED_SIZE); }
  GLint greenSize() { return param(LGL_RENDERBUFFER_GREEN_SIZE); }
  GLint blueSize() { return param(LGL_RENDERBUFFER_BLUE_SIZE); }
  GLint alphaSize() { return param(LGL_RENDERBUFFER_ALPHA_SIZE); }
  GLint depthSize() { return param(LGL_RENDERBUFFER_DEPTH_SIZE); }
  GLint stencilSize() { return param(LGL_RENDERBUFFER_STENCIL_SIZE); }

  GLint param(GLenum paramName);

  GLuint  buffer;
};
}
}

#endif
