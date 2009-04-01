#include "lost/gl/FrameBuffer.h"
#include <stdexcept>

using namespace std;

namespace lost
{
namespace gl
{

TexturePtr FrameBuffer::createColorTexture(const math::Vec2& size,
                                           GLenum internalFormat)
{
  TexturePtr result;

  Texture::Params params;
  params.internalFormat = internalFormat;
  result.reset(new Texture(size, params));
  result->filter(GL_NEAREST); // filtering needs to be off for color attachments you want to write to.
  
  return result;
}

GLenum bitDepthToEnum(uint8_t bitDepth)
{
  GLenum bitDepthEnum;
  switch(bitDepth)
  {
    case 16:bitDepthEnum=GL_DEPTH_COMPONENT16;break;
    case 24:bitDepthEnum=GL_DEPTH_COMPONENT24;break;
    case 32:bitDepthEnum=GL_DEPTH_COMPONENT32;break;
    default:throw runtime_error("bitDepth must be 16, 24 or 32");
  }
  return bitDepthEnum;
}


TexturePtr FrameBuffer::createDepthTexture(const math::Vec2& size, // size in pixels, result might vary
                                           uint8_t bitDepth)      // desired bit depth
{
  TexturePtr result;

  GLenum bitDepthEnum = bitDepthToEnum(bitDepth);
  
  result.reset(new Texture());
  result->bind();
  result->init(0,bitDepthEnum, size.width, size.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

  return result;
}



uint8_t lowerBitDepth(uint8_t bitDepth)
{
  uint8_t result = 0;
  
  switch(bitDepth)
  {
    case 32:result=24;break;
    case 24:result=16;break;
    case 16:result=0;break;
    default:result=0;
  }
  
  return result;
}

FrameBufferPtr FrameBuffer::createFrameBuffer(const math::Vec2& size,
                                        GLenum colorFormat, 
                                        uint8_t bitDepth)
{
  FrameBufferPtr result;
  
  result.reset(new FrameBuffer());
  result->enable();

  // first the color attachment
  TexturePtr tex = createColorTexture(size, colorFormat);
  result->attachColor(0, tex);
  if(!result->isComplete()) // shoot ourselves if we already fail at the very first color attachment
  {
    throw runtime_error("FBO not complete after first color attachment");
  }

  // then try to find the correct depth buffer if desired
  if(bitDepth>0)
  {
    do
    {
      DOUT("trying bitdepth: "<<(uint32_t)bitDepth);
      TexturePtr depthtex = createDepthTexture(size, bitDepth);
      result->attachDepth(depthtex);
      bitDepth = lowerBitDepth(bitDepth); // lower bitDepth should we need a next try
    }
    while((bitDepth>0) && !result->isComplete());
  }
  
  // one last check to see if we'Re complete or the depth stage failed
  if(!result->isComplete())
  {
    throw runtime_error("couldn't create FBO"); // FIXME: maybe ouput more diagnostics
  }
  
  result->disable();
  return result;
}                                        

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
  colorBuffers[index] = inRenderBuffer;
}

void FrameBuffer::attachStencil(RenderBufferPtr inRenderBuffer)
{
  attach(LGL_STENCIL_ATTACHMENT, inRenderBuffer);
  stencilBuffer = inRenderBuffer;
}

void FrameBuffer::attachDepth(RenderBufferPtr inRenderBuffer)
{
  attach(LGL_DEPTH_ATTACHMENT, inRenderBuffer);
  depthBuffer = inRenderBuffer;
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
  colorTextures[index] = inTexture;
}

void FrameBuffer::attachStencil(TexturePtr inTexture)
{
  attach(LGL_STENCIL_ATTACHMENT, inTexture);
  stencilTexture = inTexture;
}

void FrameBuffer::attachDepth(TexturePtr inTexture)
{
  attach(LGL_DEPTH_ATTACHMENT, inTexture);
  depthTexture = inTexture;
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