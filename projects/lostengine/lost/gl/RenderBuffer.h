#ifndef LOST_GL_RENDERBUFFER_H
#define LOST_GL_RENDERBUFFER_H

#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"

namespace lost
{
namespace gl
{
struct RenderBuffer
{
  RenderBuffer()
  {
    glGenRenderbuffersEXT(1, &buffer);GLDEBUG_THROW;
  }

  void enable()
  {
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, buffer);GLDEBUG_THROW;
  }

  void disable()
  {
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);GLDEBUG_THROW;
  }

  void storage(GLenum inInternalFormat, GLsizei inWidth, GLsizei inHeight)
  {
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, inInternalFormat, inWidth, inHeight);GLDEBUG_THROW;
  }

  GLint width() { return param(GL_RENDERBUFFER_WIDTH_EXT); }
  GLint height() { return param(GL_RENDERBUFFER_HEIGHT_EXT); }
  GLint internalFormat() { return param(GL_RENDERBUFFER_INTERNAL_FORMAT_EXT); }
  GLint redSize() { return param(GL_RENDERBUFFER_RED_SIZE_EXT); }
  GLint greenSize() { return param(GL_RENDERBUFFER_GREEN_SIZE_EXT); }
  GLint blueSize() { return param(GL_RENDERBUFFER_BLUE_SIZE_EXT); }
  GLint alphaSize() { return param(GL_RENDERBUFFER_ALPHA_SIZE_EXT); }
  GLint depthSize() { return param(GL_RENDERBUFFER_DEPTH_SIZE_EXT); }
  GLint stencilSize() { return param(GL_RENDERBUFFER_STENCIL_SIZE_EXT); }

  GLint param(GLenum paramName)
  {
    GLint result;
    glGetRenderbufferParameterivEXT(buffer, paramName, &result);GLDEBUG_THROW;
    return result;
  }

  virtual ~RenderBuffer()
  {
    glDeleteRenderbuffersEXT(1, &buffer);GLDEBUG;
  }

  GLuint  buffer;
};
}
}

#endif
