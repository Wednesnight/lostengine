#ifndef LOST_GL_FRAMEBUFFER_H
#define LOST_GL_FRAMEBUFFER_H

#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/gl/RenderBuffer.h"
#include "lost/gl/Texture.h"
#include <boost/shared_ptr.hpp>

namespace lost
{
namespace gl
{
struct FrameBuffer
{
  FrameBuffer()
  {
    lglGenFramebuffers(1, &buffer);GLDEBUG_THROW;
  }

  virtual ~FrameBuffer()
  {
    lglDeleteFramebuffers(1, &buffer);GLDEBUG;
  }

  void enable()
  {
    // we have to get the previous framebuffer because 0 is the default framebuffer
    // and every other framebuffer bound previous to this one becomes invalid!
    glGetIntegerv(LGL_FRAMEBUFFER_BINDING, &previousFramebuffer);GLDEBUG;
    lglBindFramebuffer(LGL_FRAMEBUFFER, buffer);GLDEBUG_THROW;
  }

  void disable()
  {
    lglBindFramebuffer(LGL_FRAMEBUFFER, previousFramebuffer);GLDEBUG;
  }

  // attach a color buffer to one of 16 slots, zero indexed
  // no need to proivde GLenum ,just index them
  void attachColor(int index, boost::shared_ptr<lost::gl::RenderBuffer> inRenderBuffer)
  {
    GLenum target;
    switch(index)
    {
      case 0:target=LGL_COLOR_ATTACHMENT0;break;
/*
      case 1:target=LGL_COLOR_ATTACHMENT1;break;
      case 2:target=LGL_COLOR_ATTACHMENT2;break;
      case 3:target=LGL_COLOR_ATTACHMENT3;break;
      case 4:target=LGL_COLOR_ATTACHMENT4;break;
      case 5:target=LGL_COLOR_ATTACHMENT5;break;
      case 6:target=LGL_COLOR_ATTACHMENT6;break;
      case 7:target=LGL_COLOR_ATTACHMENT7;break;
      case 8:target=LGL_COLOR_ATTACHMENT8;break;
      case 9:target=LGL_COLOR_ATTACHMENT9;break;
      case 10:target=LGL_COLOR_ATTACHMENT10;break;
      case 11:target=LGL_COLOR_ATTACHMENT11;break;
      case 12:target=LGL_COLOR_ATTACHMENT12;break;
      case 13:target=LGL_COLOR_ATTACHMENT13;break;
      case 14:target=LGL_COLOR_ATTACHMENT14;break;
      case 15:target=LGL_COLOR_ATTACHMENT15;break;
*/
      default: throw std::runtime_error("attachColor: index out of range");
    }
    attach(target, inRenderBuffer);
  }

  void attachStencil(boost::shared_ptr<lost::gl::RenderBuffer> inRenderBuffer)
  {
    attach(LGL_STENCIL_ATTACHMENT, inRenderBuffer);
  }

  void attachDepth(boost::shared_ptr<lost::gl::RenderBuffer> inRenderBuffer)
  {
    attach(LGL_DEPTH_ATTACHMENT, inRenderBuffer);
  }

  // attaches the renderbuffer to the currently bound framebuffer at the specified attachment target
  void attach(GLenum target, boost::shared_ptr<lost::gl::RenderBuffer> inRenderBuffer)
  {
    lglFramebufferRenderbuffer(LGL_FRAMEBUFFER, target, LGL_RENDERBUFFER, inRenderBuffer->buffer);GLDEBUG_THROW;
  }


  // attach a color buffer to one of 16 slots, zero indexed
  // no need to proivde GLenum ,just index them
  void attachColor(int index, boost::shared_ptr<lost::gl::Texture> inTexture)
  {
    GLenum target;
    switch(index)
    {
      case 0:target=LGL_COLOR_ATTACHMENT0;break;
/*
      case 1:target=LGL_COLOR_ATTACHMENT1;break;
      case 2:target=LGL_COLOR_ATTACHMENT2;break;
      case 3:target=LGL_COLOR_ATTACHMENT3;break;
      case 4:target=LGL_COLOR_ATTACHMENT4;break;
      case 5:target=LGL_COLOR_ATTACHMENT5;break;
      case 6:target=LGL_COLOR_ATTACHMENT6;break;
      case 7:target=LGL_COLOR_ATTACHMENT7;break;
      case 8:target=LGL_COLOR_ATTACHMENT8;break;
      case 9:target=LGL_COLOR_ATTACHMENT9;break;
      case 10:target=LGL_COLOR_ATTACHMENT10;break;
      case 11:target=LGL_COLOR_ATTACHMENT11;break;
      case 12:target=LGL_COLOR_ATTACHMENT12;break;
      case 13:target=LGL_COLOR_ATTACHMENT13;break;
      case 14:target=LGL_COLOR_ATTACHMENT14;break;
      case 15:target=LGL_COLOR_ATTACHMENT15;break;
*/
      default: throw std::runtime_error("attachColor: index out of range");
    }
    attach(target, inTexture);
  }

  void attachStencil(boost::shared_ptr<lost::gl::Texture> inTexture)
  {
    attach(LGL_STENCIL_ATTACHMENT, inTexture);
  }

  void attachDepth(boost::shared_ptr<lost::gl::Texture> inTexture)
  {
    attach(LGL_DEPTH_ATTACHMENT, inTexture);
  }

  void attach(GLenum target, boost::shared_ptr<lost::gl::Texture> inTexture)
  {
    GLint mipmaplevel = 0;
    lglFramebufferTexture2D(LGL_FRAMEBUFFER, target, GL_TEXTURE_2D, inTexture->texture, mipmaplevel);GLDEBUG_THROW;
  }

  GLenum status()
  {
    return lglCheckFramebufferStatus(LGL_FRAMEBUFFER);GLDEBUG_THROW;
  }

  GLuint buffer;
  GLint  previousFramebuffer;
};
}
}

#endif
