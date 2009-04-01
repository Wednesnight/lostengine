#ifndef LOST_GL_FRAMEBUFFER_H
#define LOST_GL_FRAMEBUFFER_H

#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/gl/RenderBuffer.h"
#include "lost/gl/Texture.h"
#include <boost/shared_ptr.hpp>
#include <map>

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

  // helper functions for easier creation of framebuffers, help catch common pitfalls
  // color attachment creators, either texture or renderbuffer
  // textures might have to obey certain hardware/driver limitations that renderbuffers
  // don't necessarily have to follow.
  static TexturePtr createColorTexture(const math::Vec2& size, // size in pixels, result might vary
                                       GLenum internalFormat); // format of data as stored on gfx card
  static TexturePtr createDepthTexture(const math::Vec2& size, // size in pixels, result might vary
                                       uint8_t bitDepth);      // desired bit depth, throws if bitdepth can'T be mapped to enum

  /** creates a FBO with texture attachments for color 0 and depth.
   *
   * @param size pixel width and height of FBO. Since the attachments are textures, the resulting
   *             size might vary.
   * @param colorFormat the pixel format of the data stored on the gfx card, usually GL_RGB or GL_RGBA
   * @param bitDepth the bit depth of the depth buffer. In order to ease construction of the FBO,
   *                 lower bitDepths will be tried if the initial bitDepth results in a !isComplete status.
   *
   * @return complete FBO    
   * @throws runtime_error if the FBO couldn't be constructed.
   */
  static FrameBufferPtr createFrameBuffer(const math::Vec2& size,
                                          GLenum colorFormat=GL_RGBA, // internal format of color attachment
                                          uint8_t bitDepth=24); // desired bitdepth of depth attachment
                                                             // lower resolutions will be tried if 

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
  
  // these variables hold the actual attachments so exterior code 
  // doesn't have to, and the FBO can be passed around as a single,
  // self-contained entity.
  // due to the nature an dflexibility of the whole thing, the names
  // are a bit redundant.
  // not all of these must be set. The code that actually uses the FBO 
  // should know what to expect in which slot.
  std::map<uint8_t, TexturePtr> colorTextures; // index them with the integer, NOT the GLenum
  TexturePtr depthTexture;
  TexturePtr stencilTexture;
  
  std::map<uint8_t, RenderBufferPtr> colorBuffers; // index them with the integer, NOT the GLenum
  RenderBufferPtr depthBuffer;
  RenderBufferPtr stencilBuffer;
};
}
}

#endif
