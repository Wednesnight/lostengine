#include "lost/gl/FrameBuffer.h"

namespace lost
{
namespace gl
{

void FrameBuffer::attach(GLenum target, RenderBufferPtr inRenderBuffer)
{
  lglFramebufferRenderbuffer(LGL_FRAMEBUFFER, target, LGL_RENDERBUFFER, inRenderBuffer->buffer);GLDEBUG_THROW;
}

FrameBuffer::FrameBuffer()
{
  lglGenFramebuffers(1, &buffer);GLDEBUG_THROW;
}

FrameBuffer::~FrameBuffer()
{
  lglDeleteFramebuffers(1, &buffer);GLDEBUG;
}

void FrameBuffer::enable()
{
  // we have to get the previous framebuffer because 0 is the default framebuffer
  // and every other framebuffer bound previous to this one becomes invalid!
  glGetIntegerv(LGL_FRAMEBUFFER_BINDING, &previousFramebuffer);GLDEBUG;
  lglBindFramebuffer(LGL_FRAMEBUFFER, buffer);GLDEBUG_THROW;
}

void FrameBuffer::disable()
{
  lglBindFramebuffer(LGL_FRAMEBUFFER, previousFramebuffer);GLDEBUG;
}

// attach a color buffer to one of 16 slots, zero indexed
// no need to proivde GLenum ,just index them
void FrameBuffer::attachColor(int index, RenderBufferPtr inRenderBuffer)
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

void FrameBuffer::attachStencil(RenderBufferPtr inRenderBuffer)
{
  attach(LGL_STENCIL_ATTACHMENT, inRenderBuffer);
}

void FrameBuffer::attachDepth(RenderBufferPtr inRenderBuffer)
{
  attach(LGL_DEPTH_ATTACHMENT, inRenderBuffer);
}


// attach a color buffer to one of 16 slots, zero indexed
// no need to proivde GLenum ,just index them
void FrameBuffer::attachColor(int index, TexturePtr inTexture)
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

void FrameBuffer::attachStencil(TexturePtr inTexture)
{
  attach(LGL_STENCIL_ATTACHMENT, inTexture);
}

void FrameBuffer::attachDepth(TexturePtr inTexture)
{
  attach(LGL_DEPTH_ATTACHMENT, inTexture);
}

void FrameBuffer::attach(GLenum target, TexturePtr inTexture)
{
  GLint mipmaplevel = 0;
  lglFramebufferTexture2D(LGL_FRAMEBUFFER, target, GL_TEXTURE_2D, inTexture->texture, mipmaplevel);GLDEBUG_THROW;
}

GLenum FrameBuffer::status()
{
  return lglCheckFramebufferStatus(LGL_FRAMEBUFFER);GLDEBUG_THROW;
}

bool FrameBuffer::isComplete()
{
  return (LGL_FRAMEBUFFER_COMPLETE == status());
}

}
}