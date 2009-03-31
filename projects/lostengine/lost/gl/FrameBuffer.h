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
struct FrameBuffer;
typedef boost::shared_ptr<FrameBuffer> FrameBufferPtr;

struct FrameBuffer
{
private:
  // attaches the renderbuffer to the currently bound framebuffer at the specified attachment target
  void attach(GLenum target, RenderBufferPtr inRenderBuffer);

  void attach(GLenum target, TexturePtr inTexture);


public:
  FrameBuffer();
  virtual ~FrameBuffer();

  void enable();
  void disable();

  // attach a color buffer to one of 16 slots, zero indexed
  // no need to proivde GLenum ,just index them
  void attachColor(int index, RenderBufferPtr inRenderBuffer);
  void attachStencil(RenderBufferPtr inRenderBuffer);
  void attachDepth(RenderBufferPtr inRenderBuffer);

  // attach a color buffer to one of 16 slots, zero indexed
  // no need to proivde GLenum ,just index them
  void attachColor(int index, TexturePtr inTexture);
  void attachStencil(TexturePtr inTexture);
  void attachDepth(TexturePtr inTexture);
  GLenum status();
  bool isComplete();

  GLuint buffer;
  GLint  previousFramebuffer;
};
}
}

#endif
