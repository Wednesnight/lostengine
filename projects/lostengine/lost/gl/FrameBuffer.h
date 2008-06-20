#ifndef LOST_GL_FRAMEBUFFER_H
#define LOST_GL_FRAMEBUFFER_H

#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/gl/RenderBuffer.h"
#include "lost/gl/Texture.h"

namespace lost
{
namespace gl
{
struct FrameBuffer
{
  FrameBuffer()
  {
    glGenFramebuffersEXT(1, &buffer);GLDEBUG_THROW;
  }

  virtual ~FrameBuffer()
  {
    glDeleteFramebuffersEXT(1, &buffer);GLDEBUG;
  }

  void enable()
  {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, buffer);GLDEBUG_THROW;
  }

  void disable()
  {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  }

  // attach a color buffer to one of 16 slots, zero indexed
  // no need to proivde GLenum ,just index them
  void attachColor(int index, const lost::gl::RenderBuffer& inRenderBuffer)
  {
    GLenum target;
    switch(index)
    {
      case 0:target=GL_COLOR_ATTACHMENT0_EXT;break;
      case 1:target=GL_COLOR_ATTACHMENT1_EXT;break;
      case 2:target=GL_COLOR_ATTACHMENT2_EXT;break;
      case 3:target=GL_COLOR_ATTACHMENT3_EXT;break;
      case 4:target=GL_COLOR_ATTACHMENT4_EXT;break;
      case 5:target=GL_COLOR_ATTACHMENT5_EXT;break;
      case 6:target=GL_COLOR_ATTACHMENT6_EXT;break;
      case 7:target=GL_COLOR_ATTACHMENT7_EXT;break;
      case 8:target=GL_COLOR_ATTACHMENT8_EXT;break;
      case 9:target=GL_COLOR_ATTACHMENT9_EXT;break;
      case 10:target=GL_COLOR_ATTACHMENT10_EXT;break;
      case 11:target=GL_COLOR_ATTACHMENT11_EXT;break;
      case 12:target=GL_COLOR_ATTACHMENT12_EXT;break;
      case 13:target=GL_COLOR_ATTACHMENT13_EXT;break;
      case 14:target=GL_COLOR_ATTACHMENT14_EXT;break;
      case 15:target=GL_COLOR_ATTACHMENT15_EXT;break;
      default: throw std::runtime_error("attachColor: index out of range");
    }
    attach(target, inRenderBuffer);
  }

  void attachStencil(const lost::gl::RenderBuffer& inRenderBuffer)
  {
    attach(GL_STENCIL_ATTACHMENT_EXT, inRenderBuffer);
  }

  void attachDepth(const lost::gl::RenderBuffer& inRenderBuffer)
  {
    attach(GL_DEPTH_ATTACHMENT_EXT, inRenderBuffer);
  }

  // attaches the renderbuffer to the currently bound framebuffer at the specified attachment target
  void attach(GLenum target, const lost::gl::RenderBuffer& inRenderBuffer)
  {
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, target, GL_RENDERBUFFER_EXT, inRenderBuffer.buffer);GLDEBUG_THROW;
  }


  // attach a color buffer to one of 16 slots, zero indexed
  // no need to proivde GLenum ,just index them
  void attachColor(int index, const lost::gl::Texture& inTexture)
  {
    GLenum target;
    switch(index)
    {
      case 0:target=GL_COLOR_ATTACHMENT0_EXT;break;
      case 1:target=GL_COLOR_ATTACHMENT1_EXT;break;
      case 2:target=GL_COLOR_ATTACHMENT2_EXT;break;
      case 3:target=GL_COLOR_ATTACHMENT3_EXT;break;
      case 4:target=GL_COLOR_ATTACHMENT4_EXT;break;
      case 5:target=GL_COLOR_ATTACHMENT5_EXT;break;
      case 6:target=GL_COLOR_ATTACHMENT6_EXT;break;
      case 7:target=GL_COLOR_ATTACHMENT7_EXT;break;
      case 8:target=GL_COLOR_ATTACHMENT8_EXT;break;
      case 9:target=GL_COLOR_ATTACHMENT9_EXT;break;
      case 10:target=GL_COLOR_ATTACHMENT10_EXT;break;
      case 11:target=GL_COLOR_ATTACHMENT11_EXT;break;
      case 12:target=GL_COLOR_ATTACHMENT12_EXT;break;
      case 13:target=GL_COLOR_ATTACHMENT13_EXT;break;
      case 14:target=GL_COLOR_ATTACHMENT14_EXT;break;
      case 15:target=GL_COLOR_ATTACHMENT15_EXT;break;
      default: throw std::runtime_error("attachColor: index out of range");
    }
    attach(target, inTexture);
  }

  void attachStencil(const lost::gl::Texture& inTexture)
  {
    attach(GL_STENCIL_ATTACHMENT_EXT, inTexture);
  }

  void attachDepth(const lost::gl::Texture& inTexture)
  {
    attach(GL_DEPTH_ATTACHMENT_EXT, inTexture);
  }

  void attach(GLenum target, const lost::gl::Texture& inTexture)
  {
    GLint mipmaplevel = 0;
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, target, GL_TEXTURE_2D, inTexture.texture, mipmaplevel);GLDEBUG_THROW;
  }

  GLenum status()
  {
    return glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);GLDEBUG_THROW;
  }

  GLuint buffer;
};
}
}

#endif
