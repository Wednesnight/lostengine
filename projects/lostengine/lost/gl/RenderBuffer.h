#ifndef LOST_GL_RENDERBUFFER_H
#define LOST_GL_RENDERBUFFER_H

#include "lost/gl/gltypes.h"
#include "lost/gl/forward.h"

namespace lost
{
  namespace gl
  {
    struct RenderBuffer
    {
      RenderBuffer();
      virtual ~RenderBuffer();

      void enable();
      void disable();
      void storage(GLenum inInternalFormat, GLsizei inWidth, GLsizei inHeight);

      GLint width();
      GLint height();
      GLint internalFormat();
      GLint redSize();
      GLint greenSize();
      GLint blueSize();
      GLint alphaSize();
      GLint depthSize();
      GLint stencilSize();

      GLint param(GLenum paramName);

      GLuint  buffer;
    };
  }
}

#endif
