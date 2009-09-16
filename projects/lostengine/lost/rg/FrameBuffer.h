#ifndef LOST_RG_FRAMEBUFFER_H
#define LOST_RG_FRAMEBUFFER_H

#include "lost/rg/Node.h"

namespace lost
{
namespace gl
{
struct FrameBuffer;
typedef lost::shared_ptr<FrameBuffer> FrameBufferPtr;
}
namespace rg
{

struct FrameBuffer;
typedef lost::shared_ptr<FrameBuffer> FrameBufferPtr;

struct FrameBuffer : public Node
{
  gl::FrameBufferPtr fb;
  FrameBuffer(gl::FrameBufferPtr inFb);
  static NodePtr create(gl::FrameBufferPtr inFb);
  void process(gl::ContextPtr ctx);
};

}
}

#endif
